#pragma once
#include "../global.h"
#include "Pass.h"
#include "../model/Model.h"
#include "../texture/RenderTargetTexture.h"

class ShadowMapPass : public Pass {
public:
	ShadowMapPass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera, RenderTarget *renderTarget);
	~ShadowMapPass();

	void init(Model *model, Light *light, char *textureFilename, ID3D11ShaderResourceView *depthTexture, int width, int height);
	virtual void draw();

private:
	Light *light;

	Constant* calculateLightMatrices();
};

