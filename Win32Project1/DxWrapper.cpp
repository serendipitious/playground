#include "DxWrapper.h"

D3D11_INPUT_ELEMENT_DESC inputLayout[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
};

UINT inputElementNum = ARRAYSIZE(inputLayout);

DxWrapper::DxWrapper(HWND outputWindow, int width, int height) : width(width), height(height) {
	initializeDirect3d11App(outputWindow, width, height);

	initCamera();
	pass = new Pass(d3d11Device, d3d11DevCon, camera);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	d3d11DevCon->RSSetViewports(1, &viewport);

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

	initBlendEquation();
	return true;
}

void DxWrapper::initTexture() {
	HRESULT result = D3DX11CreateShaderResourceViewFromFile(d3d11Device, "braynzar.jpg", NULL, NULL, &texture, NULL);
	validateResult(result, "create texture failed");
	
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	d3d11Device->CreateSamplerState(&samplerDesc, &textureSamplerState);
	d3d11DevCon->PSSetShaderResources(0, 1, &texture);
	d3d11DevCon->PSSetSamplers(0, 1, &textureSamplerState);
}


void DxWrapper::updateScene() {
	
}

void DxWrapper::initScene(int width, int height) {
	loadShaders();
	initModel();
	initTexture();
	initLight();
}

void DxWrapper::initLight() {
	cbPerFra.light.ambient = XMFLOAT4(0.6, 0.6, 0.6, 0.0);
	cbPerFra.light.diffuse = XMFLOAT4(1.0, 1.0, 1.0, 0.0);
	cbPerFra.light.dir = XMFLOAT3(-1.0, -1.0, -1.0);
}

void DxWrapper::initCamera() {
	camera = new Camera(XMFLOAT4(0, 3, -8, 0), XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 1, 0, 0));
}

void DxWrapper::loadShaders() {
	vertexShader = new Shader("VertexShader.hlsl", VERTEX_SHADER);
	pixelShader = new Shader("PixelShader.hlsl", PIXEL_SHADER);
	vertexShader->setShader(d3d11Device, d3d11DevCon);
	pixelShader->setShader(d3d11Device, d3d11DevCon);
}

void DxWrapper::initModel() {
	Vertex v[] = {
		// Front Face
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f),
		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, -1.0f),
		Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),

		// Back Face
		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f),
		Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f),
		Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
		Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 1.0f),

		// Top Face
		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 1.0f, -1.0f),
		Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
		Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
		Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f),

		// Bottom Face
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f),
		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 1.0f),

		// Left Face
		Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 1.0f),
		Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
		Vertex(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, -1.0f),
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f),

		// Right Face
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, -1.0f),
		Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
		Vertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f),
	};


	DWORD idx[] = {
		// Front Face
		0, 1, 2,
		0, 2, 3,

		// Back Face
		4, 5, 6,
		4, 6, 7,

		// Top Face
		8, 9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};
	pass->model = new Model(v, ARRAYSIZE(v), idx, ARRAYSIZE(idx));

	// TODO set shaders in pass's own method
	pass->model->vsBuffer = vertexShader->buffer;
	pass->IASetModel();
}

void DxWrapper::initBlendEquation() {
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	D3D11_RENDER_TARGET_BLEND_DESC targetDesc;
	ZeroMemory(&targetDesc, sizeof(targetDesc));
	targetDesc.BlendEnable = true;
	targetDesc.SrcBlend = D3D11_BLEND_SRC_COLOR;
	targetDesc.DestBlend = D3D11_BLEND_DEST_COLOR;
	targetDesc.BlendOp = D3D11_BLEND_OP_ADD;
	targetDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	targetDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	targetDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	targetDesc.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	desc.AlphaToCoverageEnable = false;
	desc.RenderTarget[0] = targetDesc;

	d3d11Device->CreateBlendState(&desc, &transparency);

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = true;
	d3d11Device->CreateRasterizerState(&rasterizerDesc, &counterClockwiseCullMode);

	rasterizerDesc.FrontCounterClockwise = false;
	d3d11Device->CreateRasterizerState(&rasterizerDesc, &clockwiseCullMode);
}


void DxWrapper::drawScene() {
	D3DXCOLOR bgColor(0.1f, 0.1f, 0.3f, 0.0f);
	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
	d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// light
	Constant* light = new Constant(&cbPerFra, sizeof(cbPerFra), 0);
	light->setConstantForPS(d3d11Device, d3d11DevCon);

	// blend
	float blendFactor[] = { 0.75f, 0.75f, 0.75f, 1.0f };
	d3d11DevCon->OMSetBlendState(0, 0, 0xffffffff);
	d3d11DevCon->OMSetBlendState(transparency, blendFactor, 0xffffffff);

	XMFLOAT4X4 view = camera->getViewMatrix();
	XMMATRIX camView = XMLoadFloat4x4(&view);
	XMMATRIX camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 1.0f, 1000.0f);
	XMMATRIX world = XMMatrixTranslation(0, 0, 5);
	XMMATRIX WVP = XMMatrixTranspose(world * camView * camProjection);

	cbPerObj.WVP = WVP;
	cbPerObj.normalTransform = XMMatrixTranspose(WVP);
	Constant *wvp = new Constant(&cbPerObj, sizeof(cbPerObj), 0);
	wvp->setConstantForVS(d3d11Device, d3d11DevCon);

	indexSize = pass->model->indexSize;
	d3d11DevCon->RSSetState(clockwiseCullMode);
	d3d11DevCon->DrawIndexed(indexSize, 0, 0);
	d3d11DevCon->RSSetState(counterClockwiseCullMode);
	d3d11DevCon->DrawIndexed(indexSize, 0, 0);

//Present the backbuffer to the screen
	swapChain->Present(0, 0);
}

bool DxWrapper::releaseDirect3d11App() {
	releaseIfNotNull(swapChain);
	releaseIfNotNull(d3d11DevCon);
	releaseIfNotNull(d3d11Device);
	releaseIfNotNull(depthStencilView);
	releaseIfNotNull(depthStencilBuffer);
	releaseIfNotNull(cbPerObjectBuffer);
	releaseIfNotNull(transparency);
	releaseIfNotNull(counterClockwiseCullMode);
	releaseIfNotNull(clockwiseCullMode);
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
