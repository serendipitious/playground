#include "Broker.h"

Broker::Broker(HWND outputWindow, int width, int height) : width(width), height(height) {
	initializeDirect3d11App(outputWindow, width, height);

	initCamera();
	pass = new Pass(d3d11Device, d3d11DevCon, camera);
	pass->loadShaders("shaders\\pass\\VertexShader.hlsl", "shaders\\pass\\PixelShader.hlsl");

	// TODO refine init view port logic
	pass->initViewport(width, height);

	Texture *texture = new Texture("resources\\braynzar.jpg", 0);
	texture->loadTexture(d3d11Device, d3d11DevCon);
	pass->addTexture(texture);


	// environment pass
	environment = new Pass(d3d11Device, d3d11DevCon, camera);
	environment->initViewport(width, height);
	environment->loadShaders("shaders\\environment\\VertexShader.hlsl", "shaders\\environment\\PixelShader.hlsl");

	SkyBox *skyBox = new SkyBox("resources\\skymap.dds", 0);
	skyBox->loadTexture(d3d11Device, d3d11DevCon);
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

	initScene(width, height);
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
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &d3d11Device, NULL, &d3d11DevCon);

	//Create our BackBuffer
	ID3D11Texture2D* BackBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	//Create our Render Target
	d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
	BackBuffer->Release();

	//Set our Render Target
	d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, NULL);

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

	d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	d3d11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
	d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	return true;
}

void Broker::updateScene() {
	
}

void Broker::initScene(int width, int height) {
	initModel();
	initLight();
}

void Broker::initLight() {
	cbPerFrame *cbPerFra = new cbPerFrame();
	cbPerFra->light.ambient = XMFLOAT4(0.6, 0.6, 0.6, 0.0);
	cbPerFra->light.diffuse = XMFLOAT4(1.0, 1.0, 1.0, 0.0);
	cbPerFra->light.dir = XMFLOAT3(-1.0, -1.0, -1.0);

	Constant* light = new Constant(cbPerFra, sizeof(cbPerFrame), 0);
	pass->addConstantForPS(light);
}

void Broker::initCamera() {
	camera = new Camera(XMFLOAT4(0, 3, -8, 0), XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 1, 0, 0));
}

void Broker::initModel() {
	pass->model = createCube();
	environment->model = CreateSphere(10, 10);
}

void Broker::drawScene() {
	D3DXCOLOR bgColor(0.1f, 0.1f, 0.3f, 0.0f);
	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
	d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	// draw sky box
	XMFLOAT4X4 identity;
	matrixBuffer *m = new matrixBuffer;
	m->matrix = XMMatrixTranspose(XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixTranslation(camera->position.x, camera->position.y, camera->position.z));
	environment->addConstantForVS(new Constant(m, sizeof(matrixBuffer), 1));
	environment->draw();

	pass->draw();

//Present the backbuffer to the screen
	swapChain->Present(0, 0);
}

bool Broker::releaseDirect3d11App() {
	releaseIfNotNull(swapChain);
	releaseIfNotNull(d3d11DevCon);
	releaseIfNotNull(d3d11Device);
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
}
