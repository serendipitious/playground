#include "Pass.h"

Pass::Pass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera)
	: device(device), context(context), camera(camera) {

}

void Pass::initViewport(int width, int height) {
	this->width = width;
	this->height = height;

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	context->RSSetViewports(1, &viewport);
}

void Pass::loadShaders(char* vsFilename, char* psFilename) {
	vertexShader = new Shader(vsFilename, VERTEX_SHADER);
	pixelShader = new Shader(psFilename, PIXEL_SHADER);
	vertexShader->setShader(device, context);
	pixelShader->setShader(device, context);
}

void Pass::IASetModel() {
	model->IASetModel(device, context, vertexShader->buffer);
}

void Pass::setConstantForPS(Constant* constant) {
	constant->setConstantForPS(device, context);
}

void Pass::setConstantForVS(Constant* constant) {
	constant->setConstantForVS(device, context);
}

void Pass::setTexture(Texture* texture) {
	texture->setTexture(device, context);
}

Pass::~Pass() {
	if (model) {
		delete model;
	}
}

void Pass::draw() {

	XMFLOAT4X4 view = camera->getViewMatrix();
	XMMATRIX camView = XMLoadFloat4x4(&view);
	XMMATRIX camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 1.0f, 1000.0f);
	XMMATRIX world = XMMatrixTranslation(0, 0, 5);
	XMMATRIX WVP = XMMatrixTranspose(world * camView * camProjection);

	cbPerObject cbPerObj;
	cbPerObj.WVP = WVP;
	cbPerObj.normalTransform = XMMatrixTranspose(WVP);
	Constant *wvp = new Constant(&cbPerObj, sizeof(cbPerObj), 0);
	this->setConstantForVS(wvp);

	int indexSize = model->dataSize;
	context->DrawIndexed(indexSize, 0, 0);
}
