#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include "Model.h"
#include "global.h"

class Pass {
public:
	Pass(ID3D11Device *device, ID3D11DeviceContext *context);
	~Pass();

	Model *model;
	void IASetModel();

	// shader buffers
	// TODO move to private
	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;
	ID3D10Blob* vsBuffer;
	ID3D10Blob* psBuffer;
	ID3D11InputLayout* layout;

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	// model buffers
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;


};

