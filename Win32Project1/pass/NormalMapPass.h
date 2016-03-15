#pragma once

#include "../global.h"
#include "Pass.h"
#include "../texture/Texture.h"

class NormalMapPass : public Pass {
public:
	NormalMapPass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera, RenderTarget *renderTarget);
	~NormalMapPass();

	void init(Model *model, char *normalMapFile, char *textureFile, Light *light, int width, int height);

private:
	Texture *normalTexture;
	Texture *texture;
	Light *light;
};

