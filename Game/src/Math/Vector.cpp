#include "stdafx.h"
#include "Vector.h"

// -------- VECTOR2F -------- //

Vector2f::Vector2f() : x(0), y(0) { }
Vector2f::Vector2f(float x, float y) : x(x), y(y) { }
Vector2f::Vector2f(const Vector3f& v) {
	x = v.x;
	y = v.y;
}

Vector2f Vector2f::operator-(const Vector2f& other) const {
	return Vector2f(x - other.x, y - other.y);
}
Vector2f Vector2f::operator+(const Vector2f& other) const {
	return Vector2f(x + other.x, y + other.y);
}
Vector2f Vector2f::operator/(const Vector2f& other) const {
	return Vector2f(x / other.x, y / other.y);
}
Vector2f Vector2f::operator*(const Vector2f& other) const {
	return Vector2f(x * other.x, y * other.y);
}
Vector2f Vector2f::operator/(float scalar) const {
	return Vector2f(x / scalar, y / scalar);
}
Vector2f Vector2f::operator*(float scalar) const {
	return Vector2f(x * scalar, y * scalar);
}
Vector2f Vector2f::operator+(float scalar) const {
	return Vector2f(x + scalar, y + scalar);
}

std::string Vector2f::to_string() const {
	return std::to_string(x) + ", " + std::to_string(y);
}

// -------- VECTOR3F -------- //

Vector3f::Vector3f() : x(0), y(0), z(0) { }
Vector3f::Vector3f(Vector2f v, float z) { 
	x = v.x;
	y = v.y;
	this->z = z;
}
Vector3f::Vector3f(float x, float y, float z) : x(x), y(y), z(z) { }

Vector3f Vector3f::operator-(float scalar) const {
	return Vector3f(x - scalar, y - scalar, z - scalar);
}

Vector3f Vector3f::operator-(const Vector3f& other) const {
	return Vector3f(x - other.x, y - other.y, z - other.z);
}

Vector3f Vector3f::operator+(const Vector3f& other) const {
	return Vector3f(x + other.x, y + other.y, z + other.z);
}

Vector3f Vector3f::operator/(float scalar) const {
	return Vector3f(x / scalar, y / scalar, z / scalar);
}

Vector3f Vector3f::operator*(float scalar) const {
	return Vector3f(x * scalar, y * scalar, z * scalar);
}

bool Vector3f::operator==(const Vector3f& other) {
	return x == other.x && y == other.y && z == other.z;
}

std::string Vector3f::to_string() const {
	return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
}


// -------- VECTOR4F -------- //

Vector4f::Vector4f() : x(0), y(0), z(0), w(0) { }
Vector4f::Vector4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
Vector4f::Vector4f(const Vector3f& other, float w) : x(other.x), y(other.y), z(other.z), w(w) {}

Vector4f Vector4f::operator+(const Vector3f& other) {
	Vector4f res;
	res.x = this->x + other.x;
	res.y = this->y + other.y;
	res.z = this->z + other.z;
	res.w = this->w;
	return res;
}

std::string Vector4f::to_string() const {
	return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w);
}