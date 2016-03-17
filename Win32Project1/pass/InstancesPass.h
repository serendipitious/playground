#pragma once

#include "Pass.h"
#include "../global.h"
#include "../model/ModelInstance.h"

class InstancesPass : public Pass {
public:
	InstancesPass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera, RenderTarget *renderTarget);
	~InstancesPass();

	void init(ModelInstance* model, int width, int height);
};

