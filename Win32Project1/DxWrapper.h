#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include "Camera.h"
#include "Model.h"
#include "Pass.h"
#include "global.h"
#include "Shader.h"
#include "Constant.h"
#include "Texture.h"

struct Light {
	XMFLOAT3 dir;
	float pad;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

struct cbPerFrame {
	Light light;
};

class DxWrapper
{
public:
	DxWrapper(HWND outputWindow, int width, int height);
	~DxWrapper();

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
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11RenderTargetView* renderTargetView;

	// new version classes
	Pass *pass;
	Pass *environment;

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
};

