#pragma once
#include "global.h"

#define VERTEX_SHADER 1
#define PIXEL_SHADER  2

class Shader {
public:
	Shader(char *, int);
	~Shader();

	void setShader(ID3D11Device*, ID3D11DeviceContext*);

	int shaderType;
	char* filename;

	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;
	ID3D10Blob* buffer;
};

