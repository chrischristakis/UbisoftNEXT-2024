#include "stdafx.h"
#include "Primitive.h"

Primitive::Primitive() : color(1, 1, 1) { }

void Primitive::PushVertex(float x, float y, float z) {
	vertices.emplace_back(Vertex{ x, y, z });
}

void Primitive::PushVertex(Vertex v) {
	vertices.emplace_back(v);
}