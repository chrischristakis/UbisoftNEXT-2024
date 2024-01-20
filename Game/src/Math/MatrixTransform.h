#pragma once

#include "Matrix.h" 
#include "Vector.h"

namespace Math {
	namespace Transform {

		// Returns a perspective matrix with the given parameters
		Mat4x4 Perspective(float fov, float aspect, float far, float near);

		Mat4x4 Translate(const Mat4x4&, const Vector3f& pos);
		Mat4x4 Scale(const Mat4x4&, const Vector3f& scale);

		Mat4x4 RotateX(const Mat4x4& mat, float degrees);
		Mat4x4 RotateY(const Mat4x4& mat, float degrees);
		Mat4x4 RotateZ(const Mat4x4& mat, float degrees);

		Mat4x4 LookAt(Vector3f position, Vector3f forward, Vector3f up);
		Mat4x4 InverseLookAt(Vector3f pos, Vector3f target, Vector3f tempUp);

	}
}