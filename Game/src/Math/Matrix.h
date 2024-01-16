#pragma once

#include "stdafx.h"
#include "Vector.h"

class Mat3x3 {
private:

	float m_matrix[3][3] = {
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
	};

public:

	Mat3x3() = default;
	Mat3x3(float identity);

	static Mat3x3 Identity();

	float &operator()(int r, int c);
	Mat3x3 operator*(const Mat3x3& other) const;
	Vector3f operator*(const Vector3f& other) const;
};

class Mat4x4 {
private:

	float m_matrix[4][4] = {
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};

public:

	Mat4x4() = default;
	Mat4x4(float identity);

	static Mat4x4 Identity();

	float &operator()(int r, int c);
	Mat4x4 operator*(const Mat4x4& other) const;
	Vector4f operator*(const Vector4f& other) const;
	bool operator==(const Mat4x4& other) const;
};