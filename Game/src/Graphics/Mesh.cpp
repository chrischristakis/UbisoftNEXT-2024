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
	Vertex tbl(-0.5f, 0.5f, -0.5f);
	Vertex tbr(0.5f, 0.5f, -0.5f);
	Vertex tfl(-0.5f, 0.5f, 0.5f);
	Vertex tfr(0.5f, 0.5f, 0.5f);
	Vertex bbl(-0.5f, -0.5f, -0.5f);
	Vertex bbr(0.5f, -0.5f, -0.5f);
	Vertex bfl(-0.5f, -0.5f, 0.5f);
	Vertex bfr(0.5f, -0.5f, 0.5f);

	Primitive front { tfr, tfl, bfl, bfr };
	Primitive back { tbr, tbl, bbl, bbr };
	Primitive left { tfl, tbl, bbl, bfl };
	Primitive right { tfr, tbr, bbr, bfr };

	// Don't need top and bottom since it's just a wireframe, they look the same.
	return { front, back, left, right };
}

static Mesh GeneratePyramid() {
	Vertex bl(-0.5f, -0.5f, -0.5f);
	Vertex br(0.5f, -0.5f, -0.5f);
	Vertex fl(-0.5f, -0.5f, 0.5f);
	Vertex fr(0.5f, -0.5f, 0.5f);
	Vertex tip(0, 0.5f, 0);

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
		Vertex v(cos(input) * 0.5f, -0.5f, sin(input) * 0.5f);
		bottomVerts.push_back(v);
	}

	// For a triangle between each bottom vert, its neighbour and the tip.
	Vertex tip(0, 0.5f, 0);
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
	Vertex tl(-0.5f, 0.5f, 0);
	Vertex tr(0.5f, 0.5f, 0);
	Vertex br(0.5f, -0.5f, 0);
	Vertex bl(-0.5f, -0.5f, 0);

	return { { tl, tr, br, bl } };
}

static Mesh GenerateStar() {
	Primitive line1{ {{-0.5f, 0, 0}, {0.5f, 0, 0}} };
	Primitive line2{ {{0, 0, -0.5f}, {0, 0, 0.5f}} };
	Primitive line3{ {{0, -0.5f, 0}, {0, 0.5f, 0}} };
	Primitive line4{ {{-0.25f, -0.25f, -0.25f}, {0.25f, 0.25f, 0.25f}} };
	Primitive line5{ {{-0.25f, -0.25f, 0.25f}, {0.25f, 0.25f, -0.25f}} };
	Primitive line6{ {{0.25f, -0.25f, -0.25f}, {-0.25f, 0.25f, 0.25f}} };
	Primitive line7{ {{-0.25f, 0.25f, -0.25f}, {0.25f, -0.25f, 0.25f}} };

	return { line1, line2, line3, line4, line5, line6, line7 };
}