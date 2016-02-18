#include "DxWrapper.h"

DxWrapper::DxWrapper(HWND outputWindow, int width, int height) : width(width), height(height) {
	initializeDirect3d11App(outputWindow, width, height);

	initCamera();
	pass = new Pass(d3d11Device, d3d11DevCon, camera);
	pass->loadShaders("shaders\\pass\\VertexShader.hlsl", "shaders\\pass\\PixelShader.hlsl");

	// TODO refine init view port logic
	pass->initViewport(width, height);

	Texture *texture = new Texture("resources\\braynzar.jpg", 0);
	pass->setTexture(texture);

	initScene(width, height);
}

bool DxWrapper::initializeDirect3d11App(HWND outputWindow, int width, int height) {
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

void DxWrapper::updateScene() {
	
}

void DxWrapper::initScene(int width, int height) {
	initModel();
	initLight();
}

void DxWrapper::initLight() {
	cbPerFrame *cbPerFra = new cbPerFrame();
	cbPerFra->light.ambient = XMFLOAT4(0.6, 0.6, 0.6, 0.0);
	cbPerFra->light.diffuse = XMFLOAT4(1.0, 1.0, 1.0, 0.0);
	cbPerFra->light.dir = XMFLOAT3(-1.0, -1.0, -1.0);

	Constant* light = new Constant(cbPerFra, sizeof(cbPerFrame), 0);
	pass->setConstantForPS(light);
}

void DxWrapper::initCamera() {
	camera = new Camera(XMFLOAT4(0, 3, -8, 0), XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 1, 0, 0));
}

void DxWrapper::initModel() {
	int vertexSize = 24;
	Vertex *v = new Vertex[24];
		// Front Face
	v[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f);
	v[1] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, -1.0f);
	v[2] = Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f);
	v[3] = Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f);

		// Back Face
	v[4] = Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
	v[5] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f);
	v[6] = Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[7] = Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 1.0f);

		// Top Face
	v[8] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 1.0f, -1.0f);
	v[9] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	v[10] = Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[11] = Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f);

		// Bottom Face
	v[12] = Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f);
	v[13] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f);
	v[14] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f);
	v[15] = Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 1.0f);

		// Left Face
	v[16] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 1.0f);
	v[17] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	v[18] = Vertex(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, -1.0f);
	v[19] = Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f);

		// Right Face
	v[20] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f);
	v[21] = Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, -1.0f);
	v[22] = Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[23] = Vertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f);


	int indexSize = 36;
	DWORD *idx = new DWORD[indexSize];
	idx[0] = 0;idx[1] = 1;idx[2] = 2;idx[3] = 0;idx[4] = 2;idx[5] = 3;
	idx[6] = 4;idx[7] = 5;idx[8] = 6;idx[9] = 4;idx[10] = 6;idx[11] = 7;
	idx[12] = 8;idx[13] = 9;idx[14] = 10;idx[15] = 8;idx[16] = 10;idx[17] = 11;
	idx[18] = 12;idx[19] = 13;idx[20] = 14;idx[21] = 12;idx[22] = 14;idx[23] = 15;
	idx[24] = 16;idx[25] = 17;idx[26] = 18;idx[27] = 16;idx[28] = 18;idx[29] = 19;
	idx[30] = 20;idx[31] = 21;idx[32] = 22;idx[33] = 20;idx[34] = 22;idx[35] = 23;

	pass->model = new Model(v, vertexSize, idx, indexSize);
}

void DxWrapper::drawScene() {
	D3DXCOLOR bgColor(0.1f, 0.1f, 0.3f, 0.0f);
	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
	d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	//environment->draw();
	pass->draw();

//Present the backbuffer to the screen
	swapChain->Present(0, 0);
}

bool DxWrapper::releaseDirect3d11App() {
	releaseIfNotNull(swapChain);
	releaseIfNotNull(d3d11DevCon);
	releaseIfNotNull(d3d11Device);
	releaseIfNotNull(depthStencilView);
	releaseIfNotNull(depthStencilBuffer);
	return true;
}

void DxWrapper::rotateLeft() {
	camera->rotateLeft();
}

void DxWrapper::rotateRight() {
	camera->rotateRight();
}

void DxWrapper::rotateUp() {
	camera->rotateUp();
}

void DxWrapper::rotateDown() {
	camera->rotateDown();
}

void DxWrapper::moveForward() {
	camera->moveForward();
}

void DxWrapper::moveBackward() {
	camera->moveBackward();
}

DxWrapper::~DxWrapper() {
	releaseDirect3d11App();
}
