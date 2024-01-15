#pragma once

#include "stdafx.h"
#include "../Math/Vector.h"

typedef Vector3f Vertex;

struct Primitive {
	std::vector<Vertex> vertices;
	Vector3f color;

	Primitive(): color(1, 1, 1) { }

	void PushVertex(float x, float y, float z) {
		vertices.emplace_back(Vertex{x, y, z});
	}

	void PushVertex(Vertex v) {
		vertices.emplace_back(v);
	}
};