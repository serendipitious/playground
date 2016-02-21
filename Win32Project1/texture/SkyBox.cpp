#include "SkyBox.h"


SkyBox::SkyBox(char *filename, int startSlot) : Texture(filename, startSlot) {
}


SkyBox::~SkyBox() {
}

void SkyBox::loadTexture(ID3D11Device *device, ID3D11DeviceContext *context) {
	D3DX11_IMAGE_LOAD_INFO loadSMInfo;
	loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* SMTexture = 0;
	HRESULT result = D3DX11CreateTextureFromFile(device, filename, &loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);
	validateResult(result, "create sky box texture failed");

	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	result = device->CreateShaderResourceView(SMTexture, &SMViewDesc, &resourceView);
	validateResult(result, "create shader resource view failed");

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&sampDesc, &textureSamplerState);
}

void SkyBox::setTexture(ID3D11Device *device, ID3D11DeviceContext *context) {
	context->PSSetShaderResources(0, 1, &resourceView);
	context->PSSetSamplers(0, 1, &textureSamplerState);
}