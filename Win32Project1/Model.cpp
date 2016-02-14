#include "Model.h"


Model::Model(Vertex* data, int dataSize, DWORD* indices, int indexSize)
	: data(data), dataSize(dataSize), indices(indices), indexSize(indexSize) {
}


Model::~Model() {
	delete data;
	delete indices;
}
