#include "NormalMapPass.h"


NormalMapPass::NormalMapPass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera, RenderTarget *renderTarget)
	: Pass(device, context, camera, renderTarget) {

}


NormalMapPass::~NormalMapPass() {
	Pass::~Pass();
}

void NormalMapPass::init(Model *model, char *normalMapFile, char *textureFile, Light *light, int width, int height) {
	this->initViewport(width, height);
	this->model = model;

	this->texture = new Texture(textureFile, 0);
	this->texture->loadTexture(device, context);
	this->addTexture(this->texture);

	this->normalTexture = new Texture(normalMapFile, 1);
	this->normalTexture->loadTexture(device, context);
	this->addTexture(this->normalTexture);

	this->loadShaders("shaders\\normalMap\\VertexShader.hlsl", "shaders\\normalMap\\PixelShader.hlsl");
	this->light = light;
	cbPerFrame *buffer = new cbPerFrame;
	buffer->light = *light;
	Constant *constant = new Constant(buffer, sizeof(cbPerFrame), 0);
	this->addConstantForPS(constant);
}
