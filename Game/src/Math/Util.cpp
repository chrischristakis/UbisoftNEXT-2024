#include "stdafx.h"
#include "Util.h"
#include "App/AppSettings.h"
#include "../Constants.h"
#include "../Graphics/Camera.h"

namespace Math {

	float RadToDeg(float rad) {
		return rad * 180.0f / PI;
	}

	float DegToRad(float deg) {
		return deg * PI / 180.0f;
	}

	float Dot(const Vector3f& v1, const Vector3f& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	float Dot(const Vector2f& v1, const Vector2f& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	float Distance(const Vector2f& v) {
		return sqrt(v.x * v.x + v.y * v.y);
	}

	Vector3f Normalize(const Vector3f& v) {
		float magnitude = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
		return v / magnitude;
	}

	Vector2f Normalize(const Vector2f& v) {
		float magnitude = sqrt(v.x * v.x + v.y * v.y);
		return v / magnitude;
	}

	Vector3f Cross(const Vector3f& v1, const Vector3f& v2) {
		float x = v1.y * v2.z - v1.z * v2.y;
		float y = v1.z * v2.x - v1.x * v2.z;
		float z = v1.x * v2.y - v1.y * v2.x;

		return Vector3f(x, y, z);
	}

	float RandomFloat(float min, float max, bool randomSign) {
		if (max <= min)
			return min;

		float randAsAFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		float res = randAsAFloat * (max - min) + min;

		if (randomSign && rand() % 2 == 0)
			res *= -1;

		return res;
	}
}