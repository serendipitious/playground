#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

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

	Vertex* data;
	int dataSize;
	DWORD* indices;
	int indexSize;

};

