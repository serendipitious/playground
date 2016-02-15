#include "Constant.h"


Constant::Constant(void* data, int byteWidth, int startSlot)
	: data(data), byteWidth(byteWidth), startSlot(startSlot) {
	buffer = NULL;
}


Constant::~Constant() {
	releaseIfNotNull(buffer);
	delete data;
}

void Constant::initBuffer(ID3D11Device* device, ID3D11DeviceContext* context) {
	if (buffer) {
		return;
	}
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = byteWidth;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT result = device->CreateBuffer(&desc, NULL, &buffer);
	validateResult(result, "create buffer for constant failed");
	context->UpdateSubresource(buffer, 0, NULL, data, 0, 0);
}

void Constant::setConstantForPS(ID3D11Device* device, ID3D11DeviceContext* context) {
	initBuffer(device, context);
	context->PSSetConstantBuffers(startSlot, 1, &buffer);
}

void Constant::setConstantForVS(ID3D11Device* device, ID3D11DeviceContext* context) {
	initBuffer(device, context);
	context->VSSetConstantBuffers(startSlot, 1, &buffer);
}