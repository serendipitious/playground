#pragma once
#include "global.h"

class Texture {
public:
	Texture(char* filename, int startSlot);
	~Texture();
	void setTexture(ID3D11Device *device, ID3D11DeviceContext *context);

private:
	char* filename;
	int startSlot;
	ID3D11ShaderResourceView* texture;
	ID3D11SamplerState* textureSamplerState;
};

