#include "stdafx.h"
#include "Matrix.h"

// -------- MAT4x4 -------- //

Mat4x4::Mat4x4(float identity) {
	_matrix[0][0] = identity;
	_matrix[1][1] = identity;
	_matrix[2][2] = identity;
	_matrix[3][3] = identity;
}

Mat4x4 Mat4x4::Identity() {
	return Mat4x4();
}

float& Mat4x4::operator()(int r, int c) {
	assert(r < 4 && r >= 0 && c < 4 && c >= 0);
	return _matrix[r][c];
}

Mat4x4 Mat4x4::operator*(const Mat4x4 & other) const {
	Mat4x4 res;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
			res(i, j) = _matrix[i][0] * other._matrix[0][j] +
				_matrix[i][1] * other._matrix[1][j] +
				_matrix[i][2] * other._matrix[2][j] +
				_matrix[i][3] * other._matrix[3][j];
		}

	return res;
}

Vector4f Mat4x4::operator*(const Vector4f & other) const {
	Vector4f res;

	res.x = _matrix[0][0] * other.x + _matrix[0][1] * other.y + _matrix[0][2] * other.z + _matrix[0][3] * other.w;
	res.y = _matrix[1][0] * other.x + _matrix[1][1] * other.y + _matrix[1][2] * other.z + _matrix[1][3] * other.w;
	res.z = _matrix[2][0] * other.x + _matrix[2][1] * other.y + _matrix[2][2] * other.z + _matrix[2][3] * other.w;
	res.w = _matrix[3][0] * other.x + _matrix[3][1] * other.y + _matrix[3][2] * other.z + _matrix[3][3] * other.w;

	return res;
}

bool Mat4x4::operator==(const Mat4x4& other) const {
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (_matrix[i][j] != other._matrix[i][j])
				return false;
	return true;
}