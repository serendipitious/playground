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

}

void Pass::IASetModel() {
	model->IASetModel(device, context, vertexShader->buffer);
}

void Pass::addConstantForPS(Constant* constant) {
	constantForPSList.push_back(constant);
}

void Pass::addConstantForVS(Constant* constant) {
	constantForVSList.push_back(constant);
}

void Pass::addTexture(Texture* texture) {
	textureList.push_back(texture);
}

Pass::~Pass() {
	if (model) {
		delete model;
	}
}

void Pass::initDraw() {

	vertexShader->setShader(device, context);
	pixelShader->setShader(device, context);

	for (std::list<Texture*>::iterator i = textureList.begin(); i != textureList.end(); i++) {
		(*i)->setTexture(device, context);
	}

	for (std::list<Constant*>::iterator i = constantForVSList.begin(); i != constantForVSList.end(); i++) {
		(*i)->setConstantForVS(device, context);
	}

	for (std::list<Constant*>::iterator i = constantForPSList.begin(); i != constantForPSList.end(); i++) {
		(*i)->setConstantForPS(device, context);
	}

	IASetModel();
}

void Pass::draw() {
	initDraw();

	XMFLOAT4X4 view = camera->getViewMatrix();
	XMMATRIX camView = XMLoadFloat4x4(&view);
	XMMATRIX camProjection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)width / height, 1.0f, 1000.0f);
	XMMATRIX world = XMMatrixTranslation(0, 0, 5);
	XMMATRIX WVP = XMMatrixTranspose(world * camView * camProjection);

	cbPerObject cbPerObj;
	cbPerObj.WVP = WVP;
	cbPerObj.normalTransform = XMMatrixTranspose(WVP);
	Constant *wvp = new Constant(&cbPerObj, sizeof(cbPerObj), 0);
	wvp->setConstantForVS(device, context);

	int indexSize = model->dataSize;
	context->DrawIndexed(indexSize, 0, 0);
}
