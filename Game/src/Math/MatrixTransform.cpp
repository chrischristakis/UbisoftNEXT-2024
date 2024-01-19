#include "stdafx.h"

#include "MatrixTransform.h"
#include "Util.h"

namespace Math {
	namespace Transform {

		Mat4x4 Perspective(float fov, float aspect, float nearPlane, float farPlane) {
			Mat4x4 perspective(0.0f);
			fov = Math::DegToRad(fov);

			perspective(0, 0) = 1.0f / (aspect * tan(fov / 2.0f));
			perspective(1, 1) = 1.0f / tan(fov / 2.0f);
			perspective(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
			perspective(2, 3) = -(2 * farPlane * nearPlane) / (farPlane - nearPlane);
			perspective(3, 2) = -1;

			return perspective;
		}

		Mat4x4 Translate(const Mat4x4& mat, const Vector3f& pos) {
			Mat4x4 res;

			res(0, 3) = pos.x;
			res(1, 3) = pos.y;
			res(2, 3) = pos.z;

			return mat * res;
		}

		Mat4x4 Scale(const Mat4x4& mat, const Vector3f& scale) {
			Mat4x4 res;

			res(0, 0) = scale.x;
			res(1, 1) = scale.y;
			res(2, 2) = scale.z;

			return mat * res;
		}

		Mat4x4 RotateX(const Mat4x4& mat, float degrees) {
			Mat4x4 res;
			float rad = Math::DegToRad(degrees);

			res(1, 1) = cos(rad);
			res(1, 2) = -sin(rad);
			res(2, 1) = sin(rad);
			res(2, 2) = cos(rad);

			return mat * res;
		}

		Mat4x4 RotateZ(const Mat4x4& mat, float degrees) {
			Mat4x4 res;
			float rad = Math::DegToRad(degrees);

			res(0, 0) = cos(rad);
			res(0, 1) = -sin(rad);
			res(1, 0) = sin(rad);
			res(1, 1) = cos(rad);

			return mat * res;
		}

		Mat4x4 RotateY(const Mat4x4& mat, float degrees) {
			Mat4x4 res;
			float rad = Math::DegToRad(degrees);

			res(0, 0) = cos(rad);
			res(0, 2) = sin(rad);
			res(2, 0) = -sin(rad);
			res(2, 2) = cos(rad);

			return mat * res;
		}

		Mat4x4 LookAt(Vector3f pos, Vector3f target, Vector3f tempUp) {
			Mat4x4 res;

			Vector3f forward = Math::Normalize(pos - target);
			Vector3f right = Math::Normalize(Math::Cross(tempUp, forward));
			Vector3f up = Math::Normalize(Math::Cross(forward, right));

			res(0, 0) = right.x;
			res(0, 1) = right.y;
			res(0, 2) = right.z;
			res(0, 3) = Math::Dot(right * -1, pos);

			res(1, 0) = up.x;
			res(1, 1) = up.y;
			res(1, 2) = up.z;
			res(1, 3) = Math::Dot(up * -1, pos);

			res(2, 0) = forward.x;
			res(2, 1) = forward.y;
			res(2, 2) = forward.z;
			res(2, 3) = Math::Dot(forward * -1, pos);

			return res;
		}

	}
}