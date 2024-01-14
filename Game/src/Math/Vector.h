#pragma once

#include "stdafx.h"

struct Vector2f {
	float x, y;

	Vector2f(): x(0), y(0) { }
	Vector2f(float x, float y): x(x), y(y) { }

	std::string to_string() {
		return std::to_string(x) + ", " + std::to_string(y);
	}
};

struct Vector3f {
	float x, y, z;

	Vector3f() : x(0), y(0), z(0) { }
	Vector3f(float x, float y, float z) : x(x), y(y), z(z) { }

	std::string to_string() {
		return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
	}
};

struct Vector4f {
	float x, y, z, w;

	Vector4f() : x(0), y(0), z(0), w(0) { }
	Vector4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

	Vector4f operator+(const Vector3f& other) {
		Vector4f res;
		res.x = this->x + other.x;
		res.y = this->y + other.y;
		res.z = this->z + other.z;
		res.w = this->w;
		return res;
	}

	std::string to_string() {
		return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w);
	}
};