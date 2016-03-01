#pragma once
#include "../global.h"
#include "Texture.h"

class RenderTargetTexture : public Texture {
public:
	RenderTargetTexture(ID3D11ShaderResourceView *shaderResourceView, int startSlot);
	~RenderTargetTexture();

	virtual void loadTexture(ID3D11Device *device, ID3D11DeviceContext *context);
	virtual void setTexture(ID3D11Device *device, ID3D11DeviceContext *context);

};

