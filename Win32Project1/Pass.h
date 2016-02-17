#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include "Model.h"
#include "global.h"
#include "Camera.h"
#include "Shader.h"
#include "Constant.h"
#include "Texture.h"

struct cbPerObject {
	XMMATRIX WVP;
	XMMATRIX normalTransform;
};

class Pass {
public:
	Pass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera);
	~Pass();

	Model *model;
	void draw();

	void IASetModel();
	void loadShaders(char* vsFilename, char* psFilename);
	void initViewport(int width, int height);
		
	void setConstantForVS(Constant* constant);
	void setConstantForPS(Constant* constant);
	void setTexture(Texture* texture);

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	Camera *camera;
	Shader *vertexShader;
	Shader *pixelShader;
	int width;
	int height;

	void initViewport();
};

