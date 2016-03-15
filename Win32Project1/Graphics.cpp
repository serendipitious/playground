#include "Graphics.h"

Graphics::Graphics(HWND outputWindow, int width, int height) : width(width), height(height) {
	initializeDirect3d11App(outputWindow, width, height);

	light.diffuse = 0.6;
	light.ambient = XMFLOAT4(0.3, 0.3, 0.3, 0.0);
	light.position = XMFLOAT3(3.0, 7.0, 10.0);

	Model *bunny = loadObjModel("resources\\bunny.3dobj", 0.01);
	Model *cube = createCube();
	Model *allModel = concatModel(createCube(), bunny, 3, 0, 0);

	model = bunny;

	initCamera();

	initPass();
	initDebugPass();

	XMMATRIX trans = XMMatrixTranslation(3, 0, 0);
	XMFLOAT4X4 transMatrix;
	XMStoreFloat4x4(&transMatrix, trans);
	pass->setWorldMatrix(transMatrix);

	skyMapPass = new SkyMapPass(device, context, camera, defaultRenderTarget);
	skyMapPass->init("resources\\skymap.dds", width, height);

	renderDepthPass = new RenderDepthPass(device, context, camera, depthRenderTarget);
	renderDepthPass->init(allModel, &light, width, height);

	shadowMapPass = new ShadowMapPass(device, context, camera, defaultRenderTarget);
	shadowMapPass->init(createPlane(), &light, "resources\\grass.jpg", depthRenderTarget->getTexture(), width, height);

	normalMapPass = new NormalMapPass(device, context, camera, defaultRenderTarget);
	normalMapPass->init(createCube(), "resources\\brick_bump.jpg", "resources\\brick.jpg", &light, width, height);
}

void Graphics::initPass() {
	pass = new Pass(device, context, camera, defaultRenderTarget);
	pass->loadShaders("shaders\\pass\\VertexShader.hlsl", "shaders\\pass\\PixelShader.hlsl");

	pass->initViewport(width, height);

	Texture *texture = new Texture("resources\\brick.jpg", 0);
	texture->loadTexture(device, context);
	pass->addTexture(texture);

	// light
	cbPerFrame *cbPerFra = new cbPerFrame();
	cbPerFra->light = light;

	Constant* lightConstant = new Constant(cbPerFra, sizeof(cbPerFrame), 0);
	pass->addConstantForPS(lightConstant);

	pass->model = model;

}

void Graphics::initDebugPass() {
	debugPass = new DebugPass(device, context, camera, defaultRenderTarget);

	debugPass->initViewport(width, height);
	debugPass->loadShaders("shaders\\debugPass\\VertexShader.hlsl", "shaders\\debugPass\\PixelShader.hlsl");

	Texture *texture = new RenderTargetTexture(depthRenderTarget->getTexture(), 0);
	texture->loadTexture(device, context);
	debugPass->addTexture(texture);
}

bool Graphics::initializeDirect3d11App(HWND outputWindow, int width, int height) {
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
	depthRenderTarget = new RenderTarget();
	depthRenderTarget->init(device, width, height);
	depthRenderTarget->setAutoClean(true);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	context->RSSetViewports(1, &viewport);

	return true;
}

void Graphics::updateScene() {
	skyMapPass->update();
}

void Graphics::initCamera() {
	camera = new Camera(XMFLOAT4(5, 5, 5, 0), XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 1, 0, 0));
}

void Graphics::drawScene() {
	D3DXCOLOR bgColor(0.1f, 0.1f, 0.3f, 0.0f);
	context->ClearRenderTargetView(renderTargetView, bgColor);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	skyMapPass->draw();

	pass->draw();
	
	renderDepthPass->draw();
	shadowMapPass->draw();

	normalMapPass->draw();

	debugPass->draw();

	//Present the backbuffer to the screen
	swapChain->Present(0, 0);
}

bool Graphics::releaseDirect3d11App() {
	releaseIfNotNull(swapChain);
	releaseIfNotNull(context);
	releaseIfNotNull(device);
	releaseIfNotNull(depthStencilView);
	releaseIfNotNull(depthStencilBuffer);
	return true;
}

void Graphics::rotateLeft() {
	camera->rotateLeft();
}

void Graphics::rotateRight() {
	camera->rotateRight();
}

void Graphics::rotateUp() {
	camera->rotateUp();
}

void Graphics::rotateDown() {
	camera->rotateDown();
}

void Graphics::moveForward() {
	camera->moveForward();
}

void Graphics::moveBackward() {
	camera->moveBackward();
}

Graphics::~Graphics() {
	releaseDirect3d11App();
	deleteIfNotNull(pass);
	deleteIfNotNull(camera);
	deleteIfNotNull(debugPass);
	deleteIfNotNull(skyMapPass);
	deleteIfNotNull(renderDepthPass);
}
