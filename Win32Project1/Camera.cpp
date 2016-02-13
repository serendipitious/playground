#include "Camera.h"

Camera::Camera(XMFLOAT4 position, XMFLOAT4 target, XMFLOAT4 up) : position(position), target(target), up(up) {
	angleStep = 0.05;
	offsetStep = 0.1;
	updateView(0, 0, 0);
}

Camera::~Camera() {
}


void Camera::updateView(float pitch, float yaw, float offset) {
	XMMATRIX rotate = XMMatrixRotationRollPitchYaw(pitch, yaw, 0);
	XMVECTOR pos = XMLoadFloat4(&position);
	XMVECTOR direction = XMLoadFloat4(&target) - pos;
	XMVECTOR realTarget = XMVector3Normalize(XMVector3TransformCoord(direction, rotate)) + pos;

	XMVECTOR realUp = XMVector3TransformCoord(XMLoadFloat4(&up), rotate);
	realUp = XMVector3Normalize(realUp);

	direction = XMVector3Normalize(realTarget - pos) * offset;
	pos += direction;
	realTarget += direction;
	XMStoreFloat4(&position, pos);
	XMStoreFloat4(&target, realTarget);
	XMStoreFloat4(&up, realUp);

	XMMATRIX view = XMMatrixLookAtLH(pos, realTarget, realUp);
	XMStoreFloat4x4(&viewMatrix, view);
}

XMFLOAT4X4 Camera::getViewMatrix() {
	return viewMatrix;
}

void Camera::rotateRight() {
	updateView(0, angleStep, 0);
}

void Camera::rotateLeft() {
	updateView(0, -angleStep, 0);
}

void Camera::rotateUp() {
	updateView(-angleStep, 0, 0);
}

void Camera::rotateDown() {
	updateView(angleStep, 0, 0);
}

void Camera::moveForward() {
	updateView(0, 0, offsetStep);
}

void Camera::moveBackward() {
	updateView(0, 0, -offsetStep);
}