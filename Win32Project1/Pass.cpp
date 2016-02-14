#include "Pass.h"

Pass::Pass(ID3D11Device *device, ID3D11DeviceContext *context)
	: device(device), context(context) {
}

void Pass::IASetModel() {
	model->IASetModel(device, context);
}

Pass::~Pass() {
	if (model) {
		delete model;
	}
}
