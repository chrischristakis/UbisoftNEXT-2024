#include "stdafx.h"
#include "Camera.h"
#include "../Math/Matrix_Transform.h"
#include "App/App.h"

Camera::Camera(): Camera(Vector3f(0, 0, 0)) { }
Camera::Camera(Vector3f pos) : Camera(pos, Vector3f(0, 1, 0), Vector3f(0, 0, -1)) { }
Camera::Camera(Vector3f pos, Vector3f up, Vector3f front): position(pos), _up(up), _front(front) {
	CalculateLookAt();
}

void Camera::CalculateLookAt() {
	_lookAt = Transform::LookAt(position, position + _front, _up);
}

void Camera::Update() {
	ProcessInput();
	CalculateLookAt();
}

const Mat4x4& Camera::GetViewMatrix() {
	return _lookAt;
}

void Camera::ProcessInput() {
	if (App::IsKeyPressed('W'))
		position.z -= 1;
	if (App::IsKeyPressed('S'))
		position.z += 1;
	if (App::IsKeyPressed('A'))
		position.x -= 1;
	if (App::IsKeyPressed('D'))
		position.x += 1;
	if (App::IsKeyPressed('R'))
		position.y += 1;
	if (App::IsKeyPressed('F'))
		position.y -= 1;
}