#pragma once

#include "global.h"

class RenderTarget {
public:
	RenderTarget();
	RenderTarget(ID3D11RenderTargetView *view, ID3D11ShaderResourceView *shaderResourceView,
		ID3D11DepthStencilView *depthStencilView);
	~RenderTarget();

	void init(ID3D11Device *device, int width, int height);

	void setRenderTarget(ID3D11DeviceContext *context);
	ID3D11ShaderResourceView *getTexture();
	void setAutoClean(BOOL autoClean);

private:
	ID3D11Texture2D* renderTargetTexture;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11ShaderResourceView* shaderResourceView;
	BOOL autoClean = false;
};

