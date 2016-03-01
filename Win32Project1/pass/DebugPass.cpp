#include "DebugPass.h"


DebugPass::DebugPass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera, RenderTarget *renderTarget)
	: Pass(device, context, camera, renderTarget) {
	initModel();
}


DebugPass::~DebugPass() {

	Pass::~Pass();
}

void DebugPass::draw() {
//	Pass::initDraw();
	Pass::draw();
}

void DebugPass::initModel() {

	// just a square locating at left top corner
	Vertex *v = new Vertex[4];
	v[0] = Vertex(-0.9f,  0.9f, 0, 0, 0, 0, 0, 1);
	v[1] = Vertex(-0.6f,  0.9f, 0, 1, 0, 0, 0, 1);
	v[2] = Vertex(-0.6f,  0.6f, 0, 1, 1, 0, 0, 1);
	v[3] = Vertex(-0.9f, 0.6f, 0, 0, 1, 0, 0, 1);

	DWORD *i = new DWORD[6];
	i[0] = 0;i[1] = 1;i[2] = 2;
	i[3] = 0;i[4] = 2;i[5] = 3;

	this->model = new Model(v, 4, i, 6);
}