#include "Texture.h"


Texture::Texture(char* filename, int startSlot) : filename(filename), startSlot(startSlot) {
}

Texture::Texture(ID3D11ShaderResourceView* texture, int startSlot) : texture(texture), startSlot(startSlot) {
}

void Texture::setTexture(ID3D11Device *device, ID3D11DeviceContext *context) {
	context->PSSetShaderResources(startSlot, 1, &texture);
	context->PSSetSamplers(startSlot, 1, &textureSamplerState);
}

void Texture::loadTexture(ID3D11Device *device, ID3D11DeviceContext *context) {
	HRESULT result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &texture, NULL);
	validateResult(result, "create texture failed");

	// default sampler status
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &textureSamplerState);
}

Texture::~Texture() {
	releaseIfNotNull(texture);
	releaseIfNotNull(textureSamplerState);
}