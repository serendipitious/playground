#include "RenderTarget.h"


RenderTarget::RenderTarget() {
}

RenderTarget::RenderTarget(ID3D11RenderTargetView *view, ID3D11ShaderResourceView *shaderResourceView,
	ID3D11DepthStencilView *depthStencilView)
	: renderTargetView(view), shaderResourceView(shaderResourceView), depthStencilView(depthStencilView) {

}

RenderTarget::~RenderTarget() {
}

void RenderTarget::setAutoClean(BOOL autoClean) {
	this->autoClean = autoClean;
}

void RenderTarget::init(ID3D11Device *device, int width, int height) {
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT result = device->CreateTexture2D(&textureDesc, NULL, &renderTargetTexture);
	validateResult(result, "create texture2d in rendertarget class failed");


	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0; 

	result = device->CreateRenderTargetView(renderTargetTexture, &renderTargetViewDesc, &renderTargetView);
	validateResult(result, "create render target view in rendertarget class failed");


	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	result = device->CreateShaderResourceView(renderTargetTexture, &shaderResourceViewDesc, &shaderResourceView);
	validateResult(result, "create shader resource view failed");

	depthStencilView = NULL;
}

void RenderTarget::setRenderTarget(ID3D11DeviceContext *context) {
	if (autoClean) {
		D3DXCOLOR bgColor(0.1f, 0.1f, 0.3f, 0.0f);
		context->ClearRenderTargetView(renderTargetView, bgColor);
	}
	context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}

ID3D11ShaderResourceView* RenderTarget::getTexture() {
	return shaderResourceView;
}