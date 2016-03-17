#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include "Camera.h"
#include "model/Model.h"
#include "pass/Pass.h"
#include "pass/DebugPass.h"
#include "global.h"
#include "Shader.h"
#include "Constant.h"
#include "texture/Texture.h"
#include "model/ModelTools.h"
#include "texture/SkyBox.h"
#include "RenderTarget.h"
#include "texture/RenderTargetTexture.h"
#include "pass/SkyMapPass.h"
#include "pass/ShadowMapPass.h"
#include "pass/RenderDepthPass.h"
#include "pass/NormalMapPass.h"
#include "model/ModelInstance.h"
#include "pass/InstancesPass.h"

class Graphics
{
public:
	Graphics(HWND outputWindow, int width, int height);
	~Graphics();

	void updateScene();
	void drawScene();

	void rotateLeft();
	void rotateRight();
	void rotateUp();
	void rotateDown();
	void moveForward();
	void moveBackward();

private:
	Camera* camera;

	// d3d apps
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* renderTargetView;

	// new version classes
	Pass *pass;
	Pass *debugPass;
	Model *model;

	SkyMapPass *skyMapPass;
	RenderDepthPass *renderDepthPass;
	ShadowMapPass *shadowMapPass;
	NormalMapPass *normalMapPass;
	InstancesPass *instancesPass;

	RenderTarget *defaultRenderTarget;
	RenderTarget *depthRenderTarget;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	Light light;

	int width;
	int height;

	int indexSize;

	bool initializeDirect3d11App(HWND outputWindow, int width, int height);
	bool releaseDirect3d11App();
	void initCamera();

	void initPass();
	void initDebugPass();
};

