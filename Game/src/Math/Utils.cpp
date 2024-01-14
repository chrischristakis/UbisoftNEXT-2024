#include "stdafx.h"
#include "Utils.h"

namespace Math {

	float RadToDeg(float rad) {
		return rad * 180.0f / PI;
	}

	float DegToRad(float deg) {
		return deg * PI / 180.0f;
	}

}