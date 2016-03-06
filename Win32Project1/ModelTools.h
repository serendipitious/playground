#pragma once

#include "Model.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

static Model* createCube() {
	int vertexSize = 24;
	Vertex *v = new Vertex[24];
	v[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f);
	v[1] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, -1.0f);
	v[2] = Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f);
	v[3] = Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f);

	v[4] = Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
	v[5] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f);
	v[6] = Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[7] = Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	v[8] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 1.0f, -1.0f);
	v[9] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	v[10] = Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[11] = Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f);

	v[12] = Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f);
	v[13] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f);
	v[14] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f);
	v[15] = Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 1.0f);

	v[16] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 1.0f);
	v[17] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	v[18] = Vertex(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, -1.0f);
	v[19] = Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f);

	v[20] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f);
	v[21] = Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, -1.0f);
	v[22] = Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[23] = Vertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f);

	int indexSize = 36;
	DWORD *idx = new DWORD[indexSize];
	idx[0] = 0; idx[1] = 1; idx[2] = 2; idx[3] = 0; idx[4] = 2; idx[5] = 3;
	idx[6] = 4; idx[7] = 5; idx[8] = 6; idx[9] = 4; idx[10] = 6; idx[11] = 7;
	idx[12] = 8; idx[13] = 9; idx[14] = 10; idx[15] = 8; idx[16] = 10; idx[17] = 11;
	idx[18] = 12; idx[19] = 13; idx[20] = 14; idx[21] = 12; idx[22] = 14; idx[23] = 15;
	idx[24] = 16; idx[25] = 17; idx[26] = 18; idx[27] = 16; idx[28] = 18; idx[29] = 19;
	idx[30] = 20; idx[31] = 21; idx[32] = 22; idx[33] = 20; idx[34] = 22; idx[35] = 23;

	return new Model(v, vertexSize, idx, indexSize);
}

static Model* createPlane() {
	Vertex *v = new Vertex[4];
	v[0] = Vertex(-10.0f, 0.0f, -10.0f, 0, 0, 0, 1, 0);
	v[1] = Vertex(-10.0f, 0.0f, 10.0f, 1, 0, 0, 1, 0);
	v[2] = Vertex(10.0f, 0.0f, 10.0f, 1, 1, 0, 1, 0);
	v[3] = Vertex(10.0f, 0.0f, -10.0f, 0, 1, 0, 1, 0);

	DWORD *i = new DWORD[6];
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	return new Model(v, 4, i, 6);
}

// from tutorial
static Model* CreateSphere(int LatLines, int LongLines) {
	int NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
	int NumSphereFaces = ((LatLines - 3)*(LongLines)* 2) + (LongLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	Vertex* vertices = new Vertex[NumSphereVertices];

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	for (DWORD i = 0; i < LatLines - 2; ++i) {
		spherePitch = (i + 1) * (3.14 / (LatLines - 1));
		XMMATRIX Rotationx = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < LongLines; ++j) {
			sphereYaw = j * (6.28 / (LongLines));
			XMMATRIX Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
			currVertPos = XMVector3Normalize(currVertPos);
			vertices[i*LongLines + j + 1].pos.x = XMVectorGetX(currVertPos);
			vertices[i*LongLines + j + 1].pos.y = XMVectorGetY(currVertPos);
			vertices[i*LongLines + j + 1].pos.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[NumSphereVertices - 1].pos.x = 0.0f;
	vertices[NumSphereVertices - 1].pos.y = 0.0f;
	vertices[NumSphereVertices - 1].pos.z = -1.0f;

	DWORD *indices = new DWORD[NumSphereFaces * 3];

	int k = 0;
	for (DWORD l = 0; l < LongLines - 1; ++l) {
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = LongLines;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < LatLines - 3; ++i) {
		for (DWORD j = 0; j < LongLines - 1; ++j) {
			indices[k] = i*LongLines + j + 1;
			indices[k + 1] = i*LongLines + j + 2;
			indices[k + 2] = (i + 1)*LongLines + j + 1;

			indices[k + 3] = (i + 1)*LongLines + j + 1;
			indices[k + 4] = i*LongLines + j + 2;
			indices[k + 5] = (i + 1)*LongLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i*LongLines) + LongLines;
		indices[k + 1] = (i*LongLines) + 1;
		indices[k + 2] = ((i + 1)*LongLines) + LongLines;

		indices[k + 3] = ((i + 1)*LongLines) + LongLines;
		indices[k + 4] = (i*LongLines) + 1;
		indices[k + 5] = ((i + 1)*LongLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < LongLines - 1; ++l) {
		indices[k] = NumSphereVertices - 1;
		indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
		indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = NumSphereVertices - 1;
	indices[k + 1] = (NumSphereVertices - 1) - LongLines;
	indices[k + 2] = NumSphereVertices - 2;

	return new Model(vertices, NumSphereVertices, indices, NumSphereFaces * 3);

}

template<class T>
static T* vectorToArray(std::vector<T> v) {
	T *arr = new T[v.size()];
	for (int i = 0; i < v.size(); i++) {
		arr[i] = v[i];
	}
	return arr;
}

static Model* loadObjModel(char *objFilename) {
	std::ifstream fin(objFilename);
	if (!fin.is_open()) {
		MessageBox(0, objFilename, "Error", MB_OK);
		exit(-1);
	}

	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	int vt = 0;
	int vn = 0;

	while (!fin.eof()) {
		char line[256];
		fin.getline(line, 256);
		std::stringstream(std::string(line));
		char flag[10];
		fin >> flag;

		if (strcmp(flag, "v") == 0) {
			float x, y, z;
			fin >> x >> y >> z;
			vertices.push_back(Vertex(x, y + 1, z, 0, 0, 0, 0, 0));
		}
		else if (strcmp(flag, "vt") == 0) {
			float u, v;
			fin >> u >> v;
			vertices[vt].texcoord = XMFLOAT2(u, v);
			vt++;
		}
		else if (strcmp(flag, "vn") == 0) {
			float nx, ny, nz;
			fin >> nx >> ny >> nz;
			vertices[vn].normal = XMFLOAT3(nx, ny, nz);
			vn++;
		}
		else if (strcmp(flag, "f") == 0) {
			char pair[30];
			for (int i = 0; i < 3; i++) {
				fin >> pair;
				int index, useless1, useless2;
				sscanf_s(pair, "%d/%d/%d", &index, &useless1, &useless2);
				indices.push_back(index - 1);
			}
		}
	}
	return new Model(vectorToArray(vertices), vertices.size(), vectorToArray(indices), indices.size());
}