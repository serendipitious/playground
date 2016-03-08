#include "Broker.h"

Broker::Broker(HWND outputWindow, int width, int height) : width(width), height(height) {
	initializeDirect3d11App(outputWindow, width, height);

	light.diffuse = 0.6;
	light.ambient = XMFLOAT4(0.5, 0.5, 0.5, 0.0);
	light.position = XMFLOAT3(3.0, 7.0, 10.0);

	initCamera();

	initPass();
	initDepthPass();
	initEnvironment();
	initGround();
	initDebugPass();
}

void Broker::initPass() {
	pass = new Pass(device, context, camera, defaultRenderTarget);
	//pass->loadShaders("shaders\\pass\\VertexShader.hlsl", "shaders\\pass\\PixelShader.hlsl");
	pass->loadShaders("shaders\\normalMap\\VertexShader.hlsl", "shaders\\normalMap\\PixelShader.hlsl");

	// TODO refine init view port logic
	pass->initViewport(width, height);

	//Texture *texture = new Texture("resources\\ArcticCondorGold.jpg", 0);
	Texture *texture = new Texture("resources\\brick.jpg", 0);
	texture->loadTexture(device, context);
	pass->addTexture(texture);

	Texture *texture1 = new Texture("resources\\brick_bump.jpg", 1);
	texture1->loadTexture(device, context);
	pass->addTexture(texture1);

	// light
	cbPerFrame *cbPerFra = new cbPerFrame();
	cbPerFra->light = light;

	Constant* light = new Constant(cbPerFra, sizeof(cbPerFrame), 0);
	pass->addConstantForPS(light);
	//pass->model = loadObjModel("resources\\ArcticCondorGold.3dobj");
	pass->model = createCube();
}

void Broker::initDepthPass() {
	depthPass = new Pass(device, context, camera, debugRenderTarget);
	depthPass->setUseDefaultWVP(FALSE);
	depthPass->loadShaders("shaders\\depthMap\\VertexShader.hlsl", "shaders\\depthMap\\PixelShader.hlsl");
	depthPass->initViewport(width, height);

	//depthPass->model = loadObjModel("resources\\ArcticCondorGold.3dobj");
	depthPass->model = createCube();

	// light
	// TODO move this code to a light class
	depthMapBuffer *buffer = new depthMapBuffer();
	buffer->lightPosition = XMFLOAT4(light.position.x, light.position.y, light.position.z, 1);
	buffer->lightProject = XMMatrixTranspose(XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)width / height, 1.0f, 1000.0f));
	XMVECTOR posVec = XMLoadFloat4(&(buffer->lightPosition));
	XMVECTOR target = XMVectorSet(0, 0, 0, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);

	buffer->lightView = XMMatrixTranspose(XMMatrixLookAtLH(posVec, target, up));
	Constant *depthBufferConstant = new Constant(buffer, sizeof(depthMapBuffer), 0);
	depthPass->addConstantForVS(depthBufferConstant);

}

void Broker::initGround() {
	ground = new Pass(device, context, camera, defaultRenderTarget);

	ground->loadShaders("shaders\\ground\\VertexShader.hlsl", "shaders\\ground\\PixelShader.hlsl");

	// TODO refine init view port logic
	ground->initViewport(width, height);

	Texture *texture = new Texture("resources\\grass.jpg", 0);
	texture->loadTexture(device, context);
	ground->addTexture(texture);

	Texture *depthTexture = new RenderTargetTexture(debugRenderTarget->getTexture(), 1);
	depthTexture->loadTexture(device, context);
	ground->addTexture(depthTexture);

	// light
	cbPerFrame *cbPerFra = new cbPerFrame();
	cbPerFra->light = light;

	Constant* lightConstant = new Constant(cbPerFra, sizeof(cbPerFrame), 0);
	ground->addConstantForPS(lightConstant);
	ground->model = createPlane();

	// light view/project matrix
	depthMapBuffer *buffer = new depthMapBuffer();
	buffer->lightPosition = XMFLOAT4(light.position.x, light.position.y, light.position.z, 1);
	buffer->lightProject = XMMatrixTranspose(XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)width / height, 1.0f, 1000.0f));
	XMVECTOR posVec = XMLoadFloat4(&(buffer->lightPosition));
	XMVECTOR target = XMVectorSet(0, 0, 0, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);

	buffer->lightView = XMMatrixTranspose(XMMatrixLookAtLH(posVec, target, up));
	Constant *depthBufferConstant = new Constant(buffer, sizeof(depthMapBuffer), 1);
	depthPass->addConstantForVS(depthBufferConstant);
}

void Broker::initEnvironment() {

	// environment pass
	environment = new Pass(device, context, camera, defaultRenderTarget);
	environment->initViewport(width, height);
	environment->loadShaders("shaders\\environment\\VertexShader.hlsl", "shaders\\environment\\PixelShader.hlsl");

	SkyBox *skyBox = new SkyBox("resources\\skymap.dds", 0);
	skyBox->loadTexture(device, context);
	environment->addTexture(skyBox);

	// none cull mode
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
	cmdesc.FrontCounterClockwise = false;
	cmdesc.CullMode = D3D11_CULL_NONE;

	environment->setRasterizerState(cmdesc);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	environment->setDepthStencilState(dssDesc);

	environment->model = CreateSphere(10, 10);
}

void Broker::initDebugPass() {
	debugPass = new DebugPass(device, context, camera, defaultRenderTarget);

	debugPass->initViewport(width, height);
	debugPass->loadShaders("shaders\\debugPass\\VertexShader.hlsl", "shaders\\debugPass\\PixelShader.hlsl");

	Texture *texture = new RenderTargetTexture(debugRenderTarget->getTexture(), 0);
	texture->loadTexture(device, context);
	debugPass->addTexture(texture);
}

bool Broker::initializeDirect3d11App(HWND outputWindow, int width, int height) {
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width = width;
	bufferDesc.Height = height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = outputWindow;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//Create our SwapChain
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &context);

	//Create our BackBuffer
	ID3D11Texture2D* BackBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	//Create our Render Target
	device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
	BackBuffer->Release();

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	defaultRenderTarget = new RenderTarget(renderTargetView, NULL, depthStencilView);

	debugRenderTarget = new RenderTarget();
	debugRenderTarget->init(device, width, height);
	debugRenderTarget->setAutoClean(true);

	return true;
}

void Broker::updateScene() {
	
}

void Broker::initCamera() {
	camera = new Camera(XMFLOAT4(5, 5, 5, 0), XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 1, 0, 0));
}

void Broker::drawScene() {
	D3DXCOLOR bgColor(0.1f, 0.1f, 0.3f, 0.0f);
	context->ClearRenderTargetView(renderTargetView, bgColor);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	// draw sky box
	XMFLOAT4X4 identity;
	matrixBuffer *m = new matrixBuffer;
	m->matrix = XMMatrixTranspose(XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixTranslation(camera->position.x, camera->position.y, camera->position.z));

	// todo change add to set
	environment->addConstantForVS(new Constant(m, sizeof(matrixBuffer), 1));
	environment->draw();

	pass->setRenderTarget(defaultRenderTarget);
	pass->draw();
	
	depthPass->draw();

	ground->draw();

	debugPass->draw();

//Present the backbuffer to the screen
	swapChain->Present(0, 0);
}

bool Broker::releaseDirect3d11App() {
	releaseIfNotNull(swapChain);
	releaseIfNotNull(context);
	releaseIfNotNull(device);
	releaseIfNotNull(depthStencilView);
	releaseIfNotNull(depthStencilBuffer);
	return true;
}

void Broker::rotateLeft() {
	camera->rotateLeft();
}

void Broker::rotateRight() {
	camera->rotateRight();
}

void Broker::rotateUp() {
	camera->rotateUp();
}

void Broker::rotateDown() {
	camera->rotateDown();
}

void Broker::moveForward() {
	camera->moveForward();
}

void Broker::moveBackward() {
	camera->moveBackward();
}

Broker::~Broker() {
	releaseDirect3d11App();
	deleteIfNotNull(pass);
	deleteIfNotNull(environment);
	deleteIfNotNull(camera);
	deleteIfNotNull(debugPass);
	deleteIfNotNull(ground);
}
