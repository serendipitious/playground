#pragma once
#include "global.h"

class Constant {
public:
	Constant(void*, int, int);
	~Constant();
	void setConstantForVS(ID3D11Device*, ID3D11DeviceContext*);
	void setConstantForPS(ID3D11Device*, ID3D11DeviceContext*);

private:
	void* data;
	int byteWidth;
	int startSlot;
	ID3D11Buffer* buffer;

	void initBuffer(ID3D11Device*, ID3D11DeviceContext*);
};

