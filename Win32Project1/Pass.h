#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include "global.h"

struct Vertex {
	XMFLOAT3 pos;
	XMFLOAT2 texcoord;
	XMFLOAT3 normal;
	Vertex() {}
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		: pos(x, y, z), texcoord(u, v), normal(nx, ny, nz) {
	}
};

struct Model {
	Model(Vertex* data, int dataSize, DWORD* indices, int indexSize)
		: data(data), dataSize(dataSize), indices(indices), indexSize(indexSize) {
	}


	Vertex* data;
	int dataSize;
	DWORD* indices;
	int indexSize;
};

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

