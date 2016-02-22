#include "Shader.h"


Shader::Shader(char *f, int t) : filename(f), shaderType(t) {
}

Shader::~Shader() {
	deleteIfNotNull(vs);
	deleteIfNotNull(ps);
	deleteIfNotNull(buffer);
}

void Shader::setShader(ID3D11Device *device, ID3D11DeviceContext *context) {
	HRESULT result;
	if (shaderType == VERTEX_SHADER) {
		result = D3DX11CompileFromFile(filename, 0, 0, "main", "vs_4_0", 0, 0, 0, &buffer, 0, 0);
		validateResult(result, "exception happens when load vertex shader");

		result = device->CreateVertexShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), NULL, &vs);
		validateResult(result, "exception when create vs");
		context->VSSetShader(vs, 0, 0);
	} else {
		result = D3DX11CompileFromFile(filename, 0, 0, "main", "ps_4_0", 0, 0, 0, &buffer, 0, 0);
		validateResult(result, "exception happens when load pixel shader");

		result = device->CreatePixelShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), NULL, &ps);
		validateResult(result, "exception when create ps");
		context->PSSetShader(ps, 0, 0);
	}
}
