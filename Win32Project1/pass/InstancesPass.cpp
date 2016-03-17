#include "InstancesPass.h"


InstancesPass::InstancesPass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera, RenderTarget *renderTarget)
	: Pass(device, context, camera, renderTarget) {
}


InstancesPass::~InstancesPass() {
}

void InstancesPass::init(ModelInstance *model, int width, int height) {
	this->model = model;
	this->initViewport(width, height);
	this->loadShaders("shaders\\instances\\VertexShader.hlsl", "shaders\\instances\\PixelShader.hlsl");
}