#pragma once

#include "../Math/Math.h"

class Camera {
private:
	Vector3f _up;
	Vector3f _front;
	Mat4x4 _lookAt;

	void CalculateLookAt();
	void ProcessInput();  // If you want keyboard control of the camera, do it here.
public:
	Vector3f position;

	Camera();
	Camera(Vector3f pos);
	Camera(Vector3f pos, Vector3f up, Vector3f front);

	void Update();

	const Mat4x4& GetViewMatrix();
};