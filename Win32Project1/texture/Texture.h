#pragma once
#include "../global.h"

class Texture {
public:
	Texture(char* filename, int startSlot);
	virtual ~Texture();
	virtual void loadTexture(ID3D11Device *device, ID3D11DeviceContext *context);
	virtual void setTexture(ID3D11Device *device, ID3D11DeviceContext *context);

protected:
	char* filename;
	int startSlot;

private:
	ID3D11ShaderResourceView* texture;
	ID3D11SamplerState* textureSamplerState;
};

