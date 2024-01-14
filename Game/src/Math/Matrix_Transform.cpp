#include "stdafx.h"
#include "Matrix_Transform.h"
#include "Utils.h"

Mat4x4 Transform::Perspective(float fov, float aspect, float near, float far) {
	Mat4x4 perspective(0.0f);
	fov = Math::DegToRad(fov);

	perspective(0, 0) = 1.0f / (aspect * tan(fov / 2.0f));
	perspective(1, 1) = 1.0f / tan(fov / 2.0f);
	perspective(2, 2) = -(far + near) / (far - near);
	perspective(2, 3) = -(2 * far * near) / (far - near);
	perspective(3, 2) = -1;

	return perspective;
}

Mat4x4 Transform::RotateX(const Mat4x4& mat, float degrees) {
	Mat4x4 res;
	float rad = Math::DegToRad(degrees);

	res(1, 1) = cos(rad);
	res(1, 2) = -sin(rad);
	res(2, 1) = sin(rad);
	res(2, 2) = cos(rad);

	return mat * res;
}

Mat4x4 Transform::RotateZ(const Mat4x4& mat, float degrees) {
	Mat4x4 res;
	float rad = Math::DegToRad(degrees);

	res(0, 0) = cos(rad);
	res(0, 1) = -sin(rad);
	res(1, 0) = sin(rad);
	res(1, 1) = cos(rad);

	return mat * res;
}

Mat4x4 Transform::RotateY(const Mat4x4& mat, float degrees) {
	Mat4x4 res;
	float rad = Math::DegToRad(degrees);

	res(0, 0) = cos(rad);
	res(0, 2) = sin(rad);
	res(2, 0) = -sin(rad);
	res(2, 2) = cos(rad);

	return mat * res;
}