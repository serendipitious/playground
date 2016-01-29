#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

class Camera
{
public:
	Camera(XMFLOAT4 position, XMFLOAT4 target, XMFLOAT4 up);
	~Camera();
	XMFLOAT4X4 getViewMatrix();
	void rotateRight();
	void rotateLeft();
	void rotateUp();
	void rotateDown();
	void moveForward();
	void moveBackward();

private:
	XMFLOAT4 position;
	XMFLOAT4 target;
	XMFLOAT4 up;
	float theta;
	float alpha;
	float angleStep;
	float positionStep;
	float positionOffset;

	void rotateHorizontally(float step);
	void rotateVertically(float step);
	void move(float step);
};

