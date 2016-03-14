#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include <windows.h>

struct cbPerObject {
	XMMATRIX WVP;
	XMMATRIX normalTransform;
	XMFLOAT4 eyePosition;
};

struct depthMapBuffer {
	XMMATRIX lightView;
	XMMATRIX lightProject;
	XMFLOAT4 lightPosition;
};

struct Light {
	XMFLOAT3 position;
	float diffuse;
	XMFLOAT4 ambient;

	depthMapBuffer* caculateLightMatrices(int width, int height) {
		depthMapBuffer *buffer = new depthMapBuffer();
		buffer->lightPosition = XMFLOAT4(this->position.x, this->position.y, this->position.z, 1);
		buffer->lightProject = XMMatrixTranspose(XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)width / height, 1.0f, 1000.0f));
		XMVECTOR posVec = XMLoadFloat4(&(buffer->lightPosition));
		XMVECTOR target = XMVectorSet(0, 0, 0, 0);
		XMVECTOR up = XMVectorSet(0, 1, 0, 0);

		buffer->lightView = XMMatrixTranspose(XMMatrixLookAtLH(posVec, target, up));
		return buffer;
	}

};

struct cbPerFrame {
	Light light;
};

struct MatrixBuffer {
	XMMATRIX matrix;
};


void static validateResult(HRESULT result, char* errorMessage) {
	if (result) {
		MessageBox(0, errorMessage, "Error", MB_OK);
		exit(-1);
	}
}

void static releaseIfNotNull(IUnknown *obj) {
	if (obj) {
		obj->Release();
	}
}

void static deleteIfNotNull(void *p) {
	if (p) {
		delete p;
	}
}