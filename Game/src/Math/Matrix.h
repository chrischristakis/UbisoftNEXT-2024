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
	Mat3x3(float identity) {
		m_matrix[0][0] = identity;
		m_matrix[1][1] = identity;
		m_matrix[2][2] = identity;
	}

	float &operator()(int r, int c) {
		assert(r < 3 && r >= 0 && c < 3 && c >= 0);
		return m_matrix[r][c];
	}

	Mat3x3 operator*(const Mat3x3& other) const {
		Mat3x3 res;

		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j) {
				res(i, j) = m_matrix[i][0] * other.m_matrix[0][j] +
							m_matrix[i][1] * other.m_matrix[1][j] +
							m_matrix[i][2] * other.m_matrix[2][j];
			}

		return res;
	}

	Vector3f operator*(const Vector3f& other) const {
		Vector3f res;

		res.x = m_matrix[0][0] * other.x + m_matrix[0][1] * other.y + m_matrix[0][2] * other.z;
		res.y = m_matrix[1][0] * other.x + m_matrix[1][1] * other.y + m_matrix[1][2] * other.z;
		res.z = m_matrix[2][0] * other.x + m_matrix[2][1] * other.y + m_matrix[2][2] * other.z;

		return res;
	}
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
	Mat4x4(float identity) {
		m_matrix[0][0] = identity;
		m_matrix[1][1] = identity;
		m_matrix[2][2] = identity;
		m_matrix[3][3] = identity;
	}

	float &operator()(int r, int c) {
		assert(r < 4 && r >= 0 && c < 4 && c >= 0);
		return m_matrix[r][c];
	}

	Mat4x4 operator*(const Mat4x4& other) const {
		Mat4x4 res;

		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j) {
				res(i, j) = m_matrix[i][0] * other.m_matrix[0][j] +
							m_matrix[i][1] * other.m_matrix[1][j] +
							m_matrix[i][2] * other.m_matrix[2][j] +
							m_matrix[i][3] * other.m_matrix[3][j];
			}

		return res;
	}

	Vector4f operator*(const Vector4f& other) const {
		Vector4f res;

		res.x = m_matrix[0][0] * other.x + m_matrix[0][1] * other.y + m_matrix[0][2] * other.z + m_matrix[0][3] * other.w;
		res.y = m_matrix[1][0] * other.x + m_matrix[1][1] * other.y + m_matrix[1][2] * other.z + m_matrix[1][3] * other.w;
		res.z = m_matrix[2][0] * other.x + m_matrix[2][1] * other.y + m_matrix[2][2] * other.z + m_matrix[2][3] * other.w;
		res.w = m_matrix[3][0] * other.x + m_matrix[3][1] * other.y + m_matrix[3][2] * other.z + m_matrix[3][3] * other.w;

		return res;
	}
};