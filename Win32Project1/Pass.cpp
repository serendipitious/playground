#include "Pass.h"

Pass::Pass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera)
	: device(device), context(context), camera(camera) {
}

void Pass::IASetModel() {
	model->IASetModel(device, context);
}

Pass::~Pass() {
	if (model) {
		delete model;
	}
}

void Pass::draw() {

}
