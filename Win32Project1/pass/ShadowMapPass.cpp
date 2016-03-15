#include "ShadowMapPass.h"

ShadowMapPass::ShadowMapPass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera, RenderTarget *renderTarget)
	: Pass(device, context, camera, renderTarget) {
}

void ShadowMapPass::init(Model *model, Light *light, char *textureFilename, ID3D11ShaderResourceView *depthTextureView, int width, int height) {
	this->model = model;
	this->light = light;
	this->initViewport(width, height);
	this->loadShaders("shaders\\shadowMap\\VertexShader.hlsl", "shaders\\shadowMap\\PixelShader.hlsl");

	Texture *texture = new Texture(textureFilename, 0);
	texture->loadTexture(device, context);
	this->addTexture(texture);

	Texture *depthTexture = new RenderTargetTexture(depthTextureView, 1);
	depthTexture->loadTexture(device, context);
	this->addTexture(depthTexture);

	Constant *lightMatrices = calculateLightMatrices();
	this->addConstantForVS(lightMatrices);

	cbPerFrame *frame = new cbPerFrame;
	frame->light = *light;
	this->addConstantForPS(new Constant(frame, sizeof(cbPerFrame), 0));
}


ShadowMapPass::~ShadowMapPass() {
}

Constant* ShadowMapPass::calculateLightMatrices() {
	depthMapBuffer *buffer = light->caculateLightMatrices(width, height);
	return new Constant(buffer, sizeof(depthMapBuffer), 1);
}

void ShadowMapPass::draw() {
	this->setConstantForVS(1, calculateLightMatrices());
	Pass::draw();
}