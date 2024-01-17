#pragma once

#include "Vector.h"

namespace Math {

	float RadToDeg(float rad);
	float DegToRad(float deg);

	float Dot(const Vector3f& v1, const Vector3f& v2);
	Vector3f Normalize(const Vector3f& v);
	Vector3f Cross(const Vector3f& v1, const Vector3f& v2);

	float RandomFloat(float min = 0.0f, float max = 1.0f, bool randomSign = false);

}