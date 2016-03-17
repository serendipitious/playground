#pragma once

#include "../global.h"
#include "Model.h"

struct InstanceType {
	XMFLOAT3 offset;
};

class ModelInstance : public Model {
public:
	ModelInstance(Model &model, InstanceType* instances, int instanceSize);
	ModelInstance(Vertex* data, int dataSize, DWORD* indices, int indexSize, InstanceType* instances, int instanceSize);
	virtual ~ModelInstance();
	
	virtual void draw(ID3D11Device *device, ID3D11DeviceContext *context, ID3D10Blob *vsBuffer);

private:
	void createBuffers(ID3D11Device *device, ID3D11DeviceContext *context, ID3D10Blob *vsBuffer);
	void setLayout(ID3D11Device *device, ID3D11DeviceContext *context, ID3D10Blob *vsBuffer);
	void setIABuffers(ID3D11DeviceContext *context);

	InstanceType* instances;
	int instanceSize;

	ID3D11Buffer* instanceBuffer;
};

