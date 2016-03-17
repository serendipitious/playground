#pragma once
#include "../global.h"

struct Vertex {
	XMFLOAT3 pos;
	XMFLOAT2 texcoord;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 binormal;
	Vertex(){}
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		: pos(x, y, z), texcoord(u, v), normal(nx, ny, nz) {}

	Vertex& operator- (const Vertex& v) {
		Vertex r = *this;
		r.pos.x -= v.pos.x;
		r.pos.y -= v.pos.y;
		r.pos.z -= v.pos.z;
		r.texcoord.x -= v.texcoord.x;
		r.texcoord.y -= v.texcoord.y;
		return r;
	}
};


class Model {
public:
	Model();
	Model(Vertex* data, int dataSize, DWORD* indices, int indexSize);
	~Model();
	virtual void draw(ID3D11Device *device, ID3D11DeviceContext *context, ID3D10Blob *vsBuffer);

	Vertex* data;
	int dataSize;
	DWORD* indices;
	int indexSize;

protected:
	ID3D11InputLayout* layout;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	void createBuffers(ID3D11Device *device, ID3D11DeviceContext *context, ID3D10Blob *vsBuffer);
	void setIABuffers(ID3D11DeviceContext *context);
	void setLayout(ID3D11Device *device, ID3D11DeviceContext *context, ID3D10Blob *vsBuffer);
};

