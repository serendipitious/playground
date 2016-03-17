#pragma once

#include "Pass.h"
#include "../global.h"
#include "../model/Model.h"
#include "../RenderTarget.h"

class DebugPass : public Pass {
public:
	DebugPass(ID3D11Device *device, ID3D11DeviceContext *context, Camera *camera, RenderTarget *renderTarget);
	virtual ~DebugPass();

	virtual void draw();

private:
	void initModel();

};

