#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include "Camera.h"
#include "Pass.h"
#include "global.h"

struct cbPerObject {
	XMMATRIX WVP;
	XMMATRIX normalTransform;
};

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

	Pass *pass;

	// shaders
	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;
	ID3D10Blob* vsBuffer;
	ID3D10Blob* psBuffer;
	ID3D11InputLayout* layout;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	// geometric buffers
	ID3D11Buffer* cbPerObjectBuffer;
	ID3D11Buffer* cbPerFrameBuffer;

	ID3D11ShaderResourceView* texture;
	ID3D11SamplerState* textureSamplerState;

	cbPerObject cbPerObj;
	cbPerFrame cbPerFra;

	// blend
	ID3D11BlendState *transparency;
	ID3D11RasterizerState *counterClockwiseCullMode;
	ID3D11RasterizerState *clockwiseCullMode;


	int width;
	int height;

	int indexSize;

	bool initializeDirect3d11App(HWND outputWindow, int width, int height);
	bool releaseDirect3d11App();
	void loadShaders();
	void initModel();
	void initTexture();
	void initCamera();
	void initBlendEquation();

	void initScene(int widht, int height);
	void initWVP(int width, int height);
	void initLight();
};

