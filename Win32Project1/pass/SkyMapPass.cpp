#include "SkyMapPass.h"

SkyMapPass::SkyMapPass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera, RenderTarget *renderTarget)
	: Pass(device, context, camera, renderTarget) {
	loadShaders("shaders\\environment\\VertexShader.hlsl", "shaders\\environment\\PixelShader.hlsl");
	this->model = CreateSphere(10, 10);

	this->initRasterizerState();
	this->initDepthStencilState();
	this->initCameraTranslationMatrix();
}

void SkyMapPass::initRasterizerState() {
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
	cmdesc.FrontCounterClockwise = false;
	cmdesc.CullMode = D3D11_CULL_NONE;

	this->setRasterizerState(cmdesc);
}

void SkyMapPass::initDepthStencilState() {
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	this->setDepthStencilState(dssDesc);
}

void SkyMapPass::initCameraTranslationMatrix() {
	MatrixBuffer *m = new MatrixBuffer;
	m->matrix = XMMatrixTranspose(XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixTranslation(camera->position.x, camera->position.y, camera->position.z));
	this->addConstantForVS(new Constant(m, sizeof(MatrixBuffer), 1));
}

void SkyMapPass::init(char *filename, int width, int height) {
	initViewport(width, height);
	SkyBox *texture = new SkyBox(filename, 0);
	texture->loadTexture(device, context);
	this->addTexture(texture);
}

void SkyMapPass::update() {
	MatrixBuffer *m = new MatrixBuffer;
	m->matrix = XMMatrixTranspose(XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixTranslation(camera->position.x, camera->position.y, camera->position.z));
	this->setConstantForVS(0, new Constant(m, sizeof(MatrixBuffer), 1));
}

SkyMapPass::~SkyMapPass() {
}
