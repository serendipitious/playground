#include "DxWrapper.h"

D3D11_INPUT_ELEMENT_DESC inputLayout[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
};

UINT inputElementNum = ARRAYSIZE(inputLayout);

DxWrapper::DxWrapper(HWND outputWindow, int width, int height) : width(width), height(height) {
	initializeDirect3d11App(outputWindow, width, height);

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


void releaseIfNotNull(IUnknown *obj) {
	if (obj) {
		obj->Release();
	}
}

void DxWrapper::updateScene() {
	
}

void DxWrapper::initScene(int width, int height) {
	loadShaders();
	initModel();
	initWVP(width, height);
	initTexture();
	initCamera();
}

void DxWrapper::initCamera() {
	camera = new Camera(XMFLOAT4(0, 3, -8, 0), XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 1, 0, 0));
}

void DxWrapper::loadShaders() {
	HRESULT result;
	result = D3DX11CompileFromFile("VertexShader.hlsl", 0, 0, "main", "vs_4_0", 0, 0, 0, &vsBuffer, 0, 0);
	validateResult(result, "exception happens when load vertex shader");

	result = D3DX11CompileFromFile("PixelShader.hlsl", 0, 0, "main", "ps_4_0", 0, 0, 0, &psBuffer, 0, 0);
	validateResult(result, "exception happens when load pixel shader");

	result = d3d11Device->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), NULL, &vs);
	validateResult(result, "exception when create vs");

	result = d3d11Device->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), NULL, &ps);
	validateResult(result, "exception when create ps");

	d3d11DevCon->VSSetShader(vs, 0, 0);
	d3d11DevCon->PSSetShader(ps, 0, 0);
}

void DxWrapper::initModel() {
	Vertex v[] = {
		Vertex(-1.0, -1.0, -1.0, 0.0, 1.0),
		Vertex(-1.0, 1.0, -1.0, 0.0, 0.0),
		Vertex(1.0, -1.0, -1.0, 1.0, 1.0),
		Vertex(1.0, 1.0, -1.0, 1.0, 0.0),
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(v);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	HRESULT result = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);
	validateResult(result, "create vertex buffer error");

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	d3d11DevCon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	DWORD idx[] = {
		0, 1, 3,
		0, 3, 2
	};
	indexSize = sizeof(idx) / sizeof(idx[0]);
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(idx);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	ZeroMemory(&indexBufferData, sizeof(indexBufferData));
	indexBufferData.pSysMem = idx;
	d3d11Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);

	d3d11DevCon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	result = d3d11Device->CreateInputLayout(inputLayout, inputElementNum, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &layout);
	validateResult(result, "create input layout failed");
	d3d11DevCon->IASetInputLayout(layout);
	d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DxWrapper::initWVP(int width, int height){
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);
}

void DxWrapper::drawScene() {
	D3DXCOLOR bgColor(0.1f, 0.1f, 0.3f, 0.0f);
	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
	d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// set camera
	/*
	XMVECTOR  camPosition = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	XMVECTOR  camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR  camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	*/
	XMFLOAT4X4 view = camera->getViewMatrix();
	XMMATRIX camView = XMLoadFloat4x4(&view);
	XMMATRIX camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 1.0f, 1000.0f);
	XMMATRIX world = XMMatrixIdentity();

	cbPerObj.WVP = XMMatrixTranspose(world * camView * camProjection);
	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

	d3d11DevCon->DrawIndexed(indexSize, 0, 0);

	//Present the backbuffer to the screen
	swapChain->Present(0, 0);
}

bool DxWrapper::releaseDirect3d11App() {
	releaseIfNotNull(swapChain);
	releaseIfNotNull(d3d11DevCon);
	releaseIfNotNull(d3d11Device);
	releaseIfNotNull(vertexBuffer);
	releaseIfNotNull(vs);
	releaseIfNotNull(ps);
	releaseIfNotNull(vsBuffer);
	releaseIfNotNull(psBuffer);
	releaseIfNotNull(layout);
	releaseIfNotNull(indexBuffer);
	releaseIfNotNull(depthStencilView);
	releaseIfNotNull(depthStencilBuffer);
	releaseIfNotNull(cbPerObjectBuffer);
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

void DxWrapper::validateResult(HRESULT result, char* errorMessage) {
	if (result) {
		MessageBox(0, errorMessage, "Error", MB_OK);
		exit(-1);
	}
}

DxWrapper::~DxWrapper() {
	releaseDirect3d11App();
}
