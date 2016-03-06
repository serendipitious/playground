#include "RenderTargetTexture.h"


RenderTargetTexture::RenderTargetTexture(ID3D11ShaderResourceView *shaderResourceView, int startSlot)
	: Texture(shaderResourceView, startSlot) {

}

RenderTargetTexture::~RenderTargetTexture() {
	Texture::~Texture();
}

void RenderTargetTexture::loadTexture(ID3D11Device *device, ID3D11DeviceContext *contex) {
	// default sampler status
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &textureSamplerState);
}

void RenderTargetTexture::setTexture(ID3D11Device *device, ID3D11DeviceContext *context) {
	context->PSSetShaderResources(startSlot, 1, &texture);
	context->PSSetSamplers(startSlot, 1, &textureSamplerState);
}