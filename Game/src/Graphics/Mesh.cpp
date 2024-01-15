#include "stdafx.h"

#include "Mesh.h"
#include "Primitive.h"

// ------- HOISTS -------- //
static Mesh GenerateCube();
static Mesh GeneratePyramid();
static Mesh GenerateQuad();

// ------- EXTERN CONSTS ------- //
const Mesh Meshes::CUBE = GenerateCube();
const Mesh Meshes::PYRAMID = GeneratePyramid();
const Mesh Meshes::QUAD = GenerateQuad();

// ------- GENERATORS ------- //

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

static Mesh GenerateQuad() {
	Vertex tl(-1, 1, 0);
	Vertex tr(1, 1, 0);
	Vertex br(1, -1, 0);
	Vertex bl(-1, -1, 0);

	return { { tl, tr, br, bl } };
}