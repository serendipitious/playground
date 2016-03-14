#include "Pass.h"

Pass::Pass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera, RenderTarget *renderTarget)
	: device(device), context(context), camera(camera), renderTarget(renderTarget) {
	vertexShader = NULL;
	pixelShader = NULL;
}

void Pass::update() {

}

void Pass::initViewport(int width, int height) {
	this->width = width;
	this->height = height;
}

void Pass::loadShaders(char* vsFilename, char* psFilename) {
	deleteIfNotNull(vertexShader);
	deleteIfNotNull(pixelShader);
	vertexShader = new Shader(vsFilename, VERTEX_SHADER);
	pixelShader = new Shader(psFilename, PIXEL_SHADER);

}

void Pass::setShaders(Shader* vs, Shader* ps) {
	vertexShader = vs;
	pixelShader = ps;
}

void Pass::IASetModel() {
	model->IASetModel(device, context, vertexShader->buffer);
}

int Pass::addConstantForPS(Constant* constant) {
	constantForPSList.push_back(constant);
	return constantForPSList.size() - 1;
}

int Pass::addConstantForVS(Constant* constant) {
	constantForVSList.push_back(constant);
	return constantForVSList.size() - 1;
}

int Pass::addTexture(Texture* texture) {
	textureList.push_back(texture);
	return textureList.size() - 1;
}

Pass::~Pass() {
	deleteIfNotNull(model);
	deleteIfNotNull(vertexShader);
	deleteIfNotNull(pixelShader);
	releaseIfNotNull(rasterizerState);
	releaseIfNotNull(depthStencilState);
	
	for (std::vector<Texture*>::iterator i = textureList.begin(); i != textureList.end(); i++) {
		deleteIfNotNull(*i);
	}

	for (std::vector<Constant*>::iterator i = constantForPSList.begin(); i != constantForPSList.end(); i++) {
		deleteIfNotNull(*i);
	}

	for (std::vector<Constant*>::iterator i = constantForVSList.begin(); i != constantForVSList.end(); i++) {
		deleteIfNotNull(*i);
	}
}

void Pass::initDraw() {
	renderTarget->setRenderTarget(context);
	vertexShader->setShader(device, context);
	pixelShader->setShader(device, context);

	for (std::vector<Texture*>::iterator i = textureList.begin(); i != textureList.end(); i++) {
		(*i)->setTexture(device, context);
	}

	for (std::vector<Constant*>::iterator i = constantForVSList.begin(); i != constantForVSList.end(); i++) {
		(*i)->setConstantForVS(device, context);
	}

	for (std::vector<Constant*>::iterator i = constantForPSList.begin(); i != constantForPSList.end(); i++) {
		(*i)->setConstantForPS(device, context);
	}

	IASetModel();

	if (rasterizerState) {
		context->RSSetState(rasterizerState);
	}

	if (depthStencilState) {
		context->OMSetDepthStencilState(depthStencilState, 0);
	}
}

void Pass::draw() {
	initDraw();
	
	if (useDefaultWVP) {
		XMFLOAT4X4 view = camera->getViewMatrix();
		XMMATRIX camView = XMLoadFloat4x4(&view);
		XMMATRIX camProjection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)width / height, 1.0f, 1000.0f);
		XMMATRIX world = XMMatrixIdentity();
		XMMATRIX WVP = XMMatrixTranspose(world * camView * camProjection);

		cbPerObject cbPerObj;
		cbPerObj.WVP = WVP;
		cbPerObj.normalTransform = world;
		cbPerObj.eyePosition = camera->position;
		Constant *wvp = new Constant(&cbPerObj, sizeof(cbPerObj), 0);
		wvp->setConstantForVS(device, context);
	}

	int indexSize = model->indexSize;
	context->DrawIndexed(indexSize, 0, 0);
}

void Pass::setRasterizerState(D3D11_RASTERIZER_DESC desc) {
	device->CreateRasterizerState(&desc, &rasterizerState);
}

void Pass::setDepthStencilState(D3D11_DEPTH_STENCIL_DESC desc) {
	device->CreateDepthStencilState(&desc, &depthStencilState);
}

void Pass::setRenderTarget(RenderTarget *renderTarget) {
	this->renderTarget = renderTarget;
}

void Pass::setUseDefaultWVP(BOOL useDefault) {
	this->useDefaultWVP = useDefault;
}

void Pass::setConstantForPS(int index, Constant* constant) {
	delete constantForPSList[index];
	constantForPSList[index] = constant;
}

void Pass::setConstantForVS(int index, Constant* constant) {
	if (index < constantForVSList.size()) {
		delete constantForVSList[index];
		constantForVSList[index] = constant;
	}
}

RenderTarget* Pass::getRenderTarget() {
	return renderTarget;
}
