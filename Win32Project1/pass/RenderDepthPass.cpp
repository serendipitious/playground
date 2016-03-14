#include "RenderDepthPass.h"


RenderDepthPass::RenderDepthPass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera, RenderTarget *renderTarget)
	: Pass(device, context, camera, renderTarget) {
}

void RenderDepthPass::init(Model *model, Light *light, int width, int height) {
	this->initViewport(width, height);
	this->model = model;
	this->loadShaders("shaders\\depthMap\\VertexShader.hlsl", "shaders\\depthMap\\PixelShader.hlsl");
	this->light = light;

	Constant *lightMatrices = calculateLightMatrices();
	this->addConstantForVS(lightMatrices);
	this->setUseDefaultWVP(false);
}

void RenderDepthPass::draw() {
	// caculate the light matrices in real time, in case that the light has changed its position
	this->setConstantForVS(0, calculateLightMatrices());
	Pass::draw();
}

RenderDepthPass::~RenderDepthPass() {
	Pass::~Pass();
}

Constant* RenderDepthPass::calculateLightMatrices() {
	depthMapBuffer *buffer = light->caculateLightMatrices(width, height);
	return new Constant(buffer, sizeof(depthMapBuffer), 0);
}
