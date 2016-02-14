#pragma once
#include "global.h"

struct Vertex {
	XMFLOAT3 pos;
	XMFLOAT2 texcoord;
	XMFLOAT3 normal;
	Vertex(){}
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		: pos(x, y, z), texcoord(u, v), normal(nx, ny, nz) {}
};


class Model {
public:
	Model(Vertex* data, int dataSize, DWORD* indices, int indexSize);
	~Model();
	void IASetModel(ID3D11Device *device, ID3D11DeviceContext *context);

	Vertex* data;
	int dataSize;
	DWORD* indices;
	int indexSize;

	// shader buffers
	// TODO move to private
	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;
	ID3D10Blob* vsBuffer;
	ID3D10Blob* psBuffer;
	ID3D11InputLayout* layout;

	// model buffers
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};

