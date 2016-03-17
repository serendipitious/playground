#include "ModelInstance.h"

D3D11_INPUT_ELEMENT_DESC instanceLayout[] =
{
	// Data from the vertex buffer
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	// Data from the instance buffer
	{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
};

UINT modelInstanceInputElementNum = ARRAYSIZE(instanceLayout);

ModelInstance::ModelInstance(Model &model, InstanceType* instances, int instanceSize)
	: Model(model), instances(instances), instanceSize(instanceSize) {
	instanceBuffer = NULL;
}

ModelInstance::ModelInstance(Vertex* data, int dataSize, DWORD* indices, int indexSize, InstanceType* instances, int instanceSize)
	: Model(data, dataSize, indices, indexSize), instances(instances), instanceSize(instanceSize) {
	instanceBuffer = NULL;
}

void ModelInstance::createBuffers(ID3D11Device *device, ID3D11DeviceContext *context, ID3D10Blob *vsBuffer) {
	Model::createBuffers(device, context, vsBuffer);

	if (!instanceBuffer) {
		// vertex buffer
		D3D11_BUFFER_DESC instanceBufferDesc;
		ZeroMemory(&instanceBufferDesc, sizeof(instanceBufferDesc));
		instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		instanceBufferDesc.ByteWidth = instanceSize * sizeof(instances[0]);
		instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instanceBufferDesc.CPUAccessFlags = 0;
		instanceBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA instanceBufferData;
		ZeroMemory(&instanceBufferData, sizeof(instanceBufferData));
		instanceBufferData.pSysMem = instances;
		HRESULT result = device->CreateBuffer(&instanceBufferDesc, &instanceBufferData, &instanceBuffer);
		validateResult(result, "create instance buffer failed");
	}
}

void ModelInstance::setLayout(ID3D11Device *device, ID3D11DeviceContext *context, ID3D10Blob *vsBuffer) {
	if (!layout) {
		HRESULT result = device->CreateInputLayout(instanceLayout, modelInstanceInputElementNum, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &layout);
		validateResult(result, "create input layout failed");
	}

	context->IASetInputLayout(layout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ModelInstance::setIABuffers(ID3D11DeviceContext *context) {
	unsigned int strides[2] = { sizeof(Vertex), sizeof(InstanceType)};
	unsigned int offsets[2] = { 0, 0 };
	ID3D11Buffer* bufferPointers[2] = { vertexBuffer, instanceBuffer };

	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
}

void ModelInstance::draw(ID3D11Device *device, ID3D11DeviceContext *context, ID3D10Blob *vsBuffer) {
	this->createBuffers(device, context, vsBuffer);
	this->setLayout(device, context, vsBuffer);
	this->setIABuffers(context);

	context->DrawIndexedInstanced(indexSize, instanceSize, 0, 0, 0);
}

ModelInstance::~ModelInstance() {
	deleteIfNotNull(instances);
	releaseIfNotNull(instanceBuffer);
	Model::~Model();
}
