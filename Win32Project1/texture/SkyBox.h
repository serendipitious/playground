#pragma once
#include "../global.h"
#include "Texture.h"

class SkyBox : public Texture {
public:
	SkyBox(char *filename, int startSlot);
	virtual ~SkyBox();
	virtual void loadTexture(ID3D11Device *device, ID3D11DeviceContext *context);
	virtual void setTexture(ID3D11Device *device, ID3D11DeviceContext *context);

private:
	ID3D11ShaderResourceView *resourceView;
	ID3D11SamplerState* textureSamplerState;
};

