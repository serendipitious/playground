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

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;

};

