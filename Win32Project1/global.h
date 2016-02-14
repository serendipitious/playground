#pragma once

#include <windows.h>

void static validateResult(HRESULT result, char* errorMessage) {
	if (result) {
		MessageBox(0, errorMessage, "Error", MB_OK);
		exit(-1);
	}
}