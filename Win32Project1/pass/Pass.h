#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include "../Model.h"
#include "../global.h"
#include "../Camera.h"
#include "../Shader.h"
#include "../Constant.h"
#include "../texture/Texture.h"
#include "../RenderTarget.h"
#include <list>

class Pass {
public:
	Pass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera, RenderTarget *renderTarget);
	virtual ~Pass();

	Model *model;
	virtual void draw();

	void IASetModel();
	void loadShaders(char* vsFilename, char* psFilename);
	void initViewport(int width, int height);
		
	void addConstantForVS(Constant* constant);
	void addConstantForPS(Constant* constant);
	void addTexture(Texture* texture);

	// states
	void setRasterizerState(D3D11_RASTERIZER_DESC desc);
	void setDepthStencilState(D3D11_DEPTH_STENCIL_DESC desc);

	void setRenderTarget(RenderTarget *renderTarget);

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	Camera *camera;
	Shader *vertexShader;
	Shader *pixelShader;
	RenderTarget *renderTarget;
	int width;
	int height;

	std::list<Texture*> textureList = std::list<Texture*>();
	std::list<Constant*> constantForVSList = std::list<Constant*>();
	std::list<Constant*> constantForPSList = std::list<Constant*>();

	ID3D11RasterizerState *rasterizerState = NULL;
	ID3D11DepthStencilState *depthStencilState = NULL;

	void initViewport();

protected:
	void initDraw();
};

