#include "Model.h"

D3D11_INPUT_ELEMENT_DESC modelInputLayout[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
UINT modelInputElementNum = ARRAYSIZE(modelInputLayout);


Model::Model() {
}

Model::Model(Vertex* data, int dataSize, DWORD* indices, int indexSize)
	: data(data), dataSize(dataSize), indices(indices), indexSize(indexSize) {
	vertexBuffer = NULL;
	indexBuffer = NULL;
	layout = NULL;
}

Model::~Model() {
	delete data;
	delete indices;
	releaseIfNotNull(vertexBuffer);
	releaseIfNotNull(indexBuffer);
	releaseIfNotNull(layout);
}

void Model::createBuffers(ID3D11Device *device, ID3D11DeviceContext *context, ID3D10Blob *vsBuffer) {
	HRESULT result;
	if (!vertexBuffer) {
		// vertex buffer
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = dataSize * sizeof(data[0]);
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		vertexBufferData.pSysMem = data;
		HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);
		validateResult(result, "create vertex buffer failed");
	}

	if (!indexBuffer) {
		// index buffer
		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = indexSize * sizeof(indices[0]);
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA indexBufferData;
		ZeroMemory(&indexBufferData, sizeof(indexBufferData));
		indexBufferData.pSysMem = indices;
		result = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);
		validateResult(result, "create index buffer failed");
	}
}

void Model::draw(ID3D11Device *device, ID3D11DeviceContext *context, ID3D10Blob *vsBuffer) {
	createBuffers(device, context, vsBuffer);
	setLayout(device, context, vsBuffer);
	setIABuffers(context);
	context->DrawIndexed(indexSize, 0, 0);
}
void Model::setIABuffers(ID3D11DeviceContext *context) {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void Model::setLayout(ID3D11Device *device, ID3D11DeviceContext *context, ID3D10Blob *vsBuffer) {
	if (!layout) {
		HRESULT result = device->CreateInputLayout(modelInputLayout, modelInputElementNum, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &layout);
		validateResult(result, "create input layout failed");
	}

	context->IASetInputLayout(layout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}