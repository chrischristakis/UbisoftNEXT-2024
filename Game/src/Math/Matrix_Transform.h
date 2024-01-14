#pragma once

#include "stdafx.h"
#include "Matrix.h" 

namespace Transform {

	// Returns a perspective matrix with the given parameters
	Mat4x4 Perspective(float fov, float aspect, float far, float near);

	Mat4x4 RotateX(const Mat4x4& mat, float degrees);
	Mat4x4 RotateY(const Mat4x4& mat, float degrees);
	Mat4x4 RotateZ(const Mat4x4& mat, float degrees);

}