#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include "../model/Model.h"
#include "../global.h"
#include "../Camera.h"
#include "../Shader.h"
#include "../Constant.h"
#include "../texture/Texture.h"
#include "../RenderTarget.h"
#include <vector>

class Pass {
public:
	Pass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera, RenderTarget *renderTarget);
	virtual ~Pass();

	Model *model;
	virtual void draw();
	virtual void update();

	void setUseDefaultWVP(BOOL useDefault);

	// deprecated
	void loadShaders(char* vsFilename, char* psFilename);
	void setShaders(Shader* vs, Shader *ps);
	void initViewport(int width, int height);
		
	void setConstantForPS(int startSlot, Constant* constant);
	void setConstantForVS(int startSlot, Constant* constant);

	int addConstantForVS(Constant* constant);
	int addConstantForPS(Constant* constant);
	int addTexture(Texture* texture);

	// states
	void setRasterizerState(D3D11_RASTERIZER_DESC desc);
	void setDepthStencilState(D3D11_DEPTH_STENCIL_DESC desc);

	void setRenderTarget(RenderTarget *renderTarget);
	RenderTarget* getRenderTarget();
	void setWorldMatrix(XMFLOAT4X4 world);

private:
	Shader *vertexShader;
	Shader *pixelShader;

	XMFLOAT4X4 world;

	std::vector<Texture*> textureList = std::vector<Texture*>();
	std::vector<Constant*> constantForVSList = std::vector<Constant*>();
	std::vector<Constant*> constantForPSList = std::vector<Constant*>();

	ID3D11RasterizerState *rasterizerState = NULL;
	ID3D11DepthStencilState *depthStencilState = NULL;
	BOOL useDefaultWVP = TRUE;

	void initViewport();


protected:
	int width;
	int height;
	Camera *camera;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	RenderTarget *renderTarget;

	void initDraw();
};

