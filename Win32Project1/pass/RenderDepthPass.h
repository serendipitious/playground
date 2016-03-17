#pragma once
#include "../global.h"
#include "Pass.h"
#include "../model/Model.h"
#include "../Constant.h"

class RenderDepthPass : public Pass {
public:
	RenderDepthPass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera, RenderTarget *renderTarget);
	~RenderDepthPass();

	void init(Model *model, Light *light, int width, int height);
	virtual void draw();

private:
	Light *light;

	Constant* calculateLightMatrices();
};

