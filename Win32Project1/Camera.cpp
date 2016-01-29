#include "Camera.h"

Camera::Camera(XMFLOAT4 position, XMFLOAT4 target, XMFLOAT4 up) : position(position), target(target), up(up) {
	theta = 0;
	alpha = 0;
	angleStep = 0.1;
	positionStep = 0.1;
	positionOffset = 0;
}

Camera::~Camera() {
}

XMFLOAT4X4 Camera::getViewMatrix() {
	XMMATRIX view = XMMatrixLookAtLH(XMLoadFloat4(&position), XMLoadFloat4(&target), XMLoadFloat4(&up));
	XMVECTOR direction = XMVector4Normalize(XMLoadFloat4(&position) - XMLoadFloat4(&target)) * positionOffset;
	XMFLOAT4 d;
	XMStoreFloat4(&d, direction);

	XMMATRIX m = XMMatrixRotationX(alpha) * XMMatrixRotationY(theta) * XMMatrixTranslation(d.x, d.y, d.z);
	view = m * view;
	XMFLOAT4X4 result;
	XMStoreFloat4x4(&result, view);
	return result;
}

void Camera::rotateRight() {
	rotateHorizontally(angleStep);
}

void Camera::rotateLeft() {
	rotateHorizontally(-angleStep);
}

void Camera::rotateUp() {
	rotateVertically(angleStep);
}

void Camera::rotateDown() {
	rotateVertically(-angleStep);
}

void Camera::moveForward() {
	move(positionStep);
}

void Camera::moveBackward() {
	move(-positionStep);
}

void Camera::rotateHorizontally(float step) {
	theta += step;
}

void Camera::rotateVertically(float step) {
	alpha += step;
}

void Camera::move(float step) {
	positionOffset += step;
}