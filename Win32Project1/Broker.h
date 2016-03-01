#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include "Camera.h"
#include "Model.h"
#include "pass/Pass.h"
#include "pass/DebugPass.h"
#include "global.h"
#include "Shader.h"
#include "Constant.h"
#include "texture/Texture.h"
#include "ModelTools.h"
#include "texture/SkyBox.h"
#include "RenderTarget.h"
#include "texture/RenderTargetTexture.h"

class Broker
{
public:
	Broker(HWND outputWindow, int width, int height);
	~Broker();

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
	Pass *environment;
	Pass *debugPass;

	RenderTarget *defaultRenderTarget;
	RenderTarget *debugRenderTarget;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	int width;
	int height;

	int indexSize;

	bool initializeDirect3d11App(HWND outputWindow, int width, int height);
	bool releaseDirect3d11App();
	void initModel();
	void initCamera();

	void initScene(int widht, int height);
	void initLight();

	void initPass();
	void initEnvironment();
	void initDebugPass();
};
