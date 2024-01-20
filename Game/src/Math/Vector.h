#pragma once

#include "stdafx.h"

struct Vector3f;

struct Vector2f {
	float x, y;

	Vector2f();
	Vector2f(float x, float y);
	Vector2f(const Vector3f& v);

	Vector2f operator-(const Vector2f& other) const;
	Vector2f operator+(const Vector2f& other) const;
	Vector2f operator/(const Vector2f& other) const;
	Vector2f operator*(const Vector2f& other) const;
	Vector2f operator/(float scalar) const;
	Vector2f operator*(float scalar) const;
	Vector2f operator+(float scalar) const;

	std::string to_string() const;
};

struct Vector3f {
	float x, y, z;

	Vector3f();
	Vector3f(Vector2f v, float z);
	Vector3f(float x, float y, float z);

	Vector3f operator-(float scalar) const;
	Vector3f operator-(const Vector3f& other) const;
	Vector3f operator+(const Vector3f& other) const;
	Vector3f operator/(float scalar) const;
	Vector3f operator*(float scalar) const;
	bool operator==(const Vector3f& other);

	std::string to_string() const;
};

struct Vector4f {
	float x, y, z, w;

	Vector4f();
	Vector4f(float x, float y, float z, float w);
	Vector4f(const Vector3f& other, float w);

	Vector4f operator+(const Vector3f& other);

	std::string to_string() const;
};