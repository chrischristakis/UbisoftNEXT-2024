#include "stdafx.h"

#include "Mesh.h"
#include "Primitive.h"
#include "../Math/Util.h"

// ------- HOISTS -------- //
static Mesh GenerateCube();
static Mesh GeneratePyramid();
static Mesh GenerateCone(int);
static Mesh GenerateQuad();
static Mesh GenerateStar();

// ------- EXTERN CONSTS ------- //
const Mesh Meshes::CUBE = GenerateCube();
const Mesh Meshes::PYRAMID = GeneratePyramid();
const Mesh Meshes::CONE = GenerateCone(7);
const Mesh Meshes::QUAD = GenerateQuad();
const Mesh Meshes::STAR = GenerateStar();

// ------- GENERATORS ------- //
// * Internal linkage

static Mesh GenerateCube() {
	Vertex tbl(-1, 1, -1);
	Vertex tbr(1, 1, -1);
	Vertex tfl(-1, 1, 1);
	Vertex tfr(1, 1, 1);
	Vertex bbl(-1, -1, -1);
	Vertex bbr(1, -1, -1);
	Vertex bfl(-1, -1, 1);
	Vertex bfr(1, -1, 1);

	Primitive front { tfr, tfl, bfl, bfr };
	Primitive back { tbr, tbl, bbl, bbr };
	Primitive left { tfl, tbl, bbl, bfl };
	Primitive right { tfr, tbr, bbr, bfr };

	// Don't need top and bottom since it's just a wireframe, they look the same.
	return { front, back, left, right };
}

static Mesh GeneratePyramid() {
	Vertex bl(-1, -1, -1);
	Vertex br(1, -1, -1);
	Vertex fl(-1, -1, 1);
	Vertex fr(1, -1, 1);
	Vertex tip(0, 1, 0);

	Primitive left { bl, fl, tip };
	Primitive right { br, fr, tip };
	Primitive front { fr, fl, tip };
	Primitive back { br, bl, tip };

	return { left, right, front, back };
}

static Mesh GenerateCone(int numBottomVerts) {

	std::vector<Vertex> bottomVerts;

	// Create base of the cone
	float increment = 360.0f / numBottomVerts;
	for (int i = 0; i < numBottomVerts; i++) {
		float input = i * Math::DegToRad(increment);
		Vertex v(cos(input), -1, sin(input));
		bottomVerts.push_back(v);
	}

	// For a triangle between each bottom vert, its neighbour and the tip.
	Vertex tip(0, 1, 0);
	Mesh res;
	for (int i = 0; i < bottomVerts.size(); i++) {
		Primitive face;

		face.PushVertex(bottomVerts[i]);
		face.PushVertex(bottomVerts[(i + 1) % bottomVerts.size()]);
		face.PushVertex(tip);

		res.push_back(face);
	}

	return res;
}

static Mesh GenerateQuad() {
	Vertex tl(-1, 1, 0);
	Vertex tr(1, 1, 0);
	Vertex br(1, -1, 0);
	Vertex bl(-1, -1, 0);

	return { { tl, tr, br, bl } };
}

static Mesh GenerateStar() {
	Primitive line1{ {{-1, 0, 0}, {1, 0, 0}} };
	Primitive line2{ {{0, 0, -1}, {0, 0, 1}} };
	Primitive line3{ {{0, -1, 0}, {0, 1, 0}} };
	Primitive line4{ {{-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}} };
	Primitive line5{ {{-0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, -0.5f}} };
	Primitive line6{ {{0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, 0.5f}} };
	Primitive line7{ {{-0.5f, 0.5f, -0.5f}, {0.5f, -0.5f, 0.5f}} };

	return { line1, line2, line3, line4, line5, line6, line7 };
}