#pragma once

#include "stdafx.h"
#include "../Math/Vector.h"

typedef Vector3f Vertex;

struct Primitive {
	std::vector<Vertex> vertices;
	Vector3f color;

	Primitive();
	Primitive(std::initializer_list<Vertex> verts);

	void PushVertex(float x, float y, float z);
	void PushVertex(Vertex v);
};