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

struct Light {
	XMFLOAT3 position;
	float diffuse;
	XMFLOAT4 ambient;
};

struct cbPerFrame {
	Light light;
};

struct matrixBuffer {
	XMMATRIX matrix;
};

struct depthMapBuffer {
	XMMATRIX lightView;
	XMMATRIX lightProject;
	XMFLOAT4 lightPosition;
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