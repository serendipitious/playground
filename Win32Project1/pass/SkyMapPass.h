#pragma once

#include "../global.h"
#include "Pass.h"
#include "../texture/SkyBox.h"
#include "../model/ModelTools.h"

class SkyMapPass : public Pass {
public:
	SkyMapPass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera, RenderTarget *renderTarge);
	virtual ~SkyMapPass();

	virtual void update();
	void init(char* filename, int width, int height);

private:
	void initRasterizerState();
	void initDepthStencilState();

	// make the model's orgin always in the middle
	void initCameraTranslationMatrix();

};

