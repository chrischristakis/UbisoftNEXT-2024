#pragma once

#include "../Math/Matrix.h"
#include "Mesh.h"
#include "Camera.h"

// A polygon with an overall depth value used for painter's algorithm.
struct PrimitiveAvgDepth: Primitive {
	float depth = 0.0f;
};

class Graphics {
private:
	const float _near = 0.1f, _far = 100.0f;
	std::vector<PrimitiveAvgDepth> _primsToRender;
	Mat4x4 _projection;
	Camera* _camera;

	bool _colorOverride;  // Override Primitive colours with MeshColor when toggled on.
	Vector3f _meshColor;

	void SortOnDepth(); // Primitives with a higher depth value get drawn first.
public:
	Graphics(Camera* camera);

	// Rendering a mesh simply applies the MVP to the primitive, then
	// it stores them in a vector so we can order them due to the
	// lack of a depth buffer, so we render furthest -> closest.
	void RenderMesh(const Mesh& m, const Mat4x4& model);
	void Update();
	void Flush();

	void SetColorOverride(bool);
	void SetMeshColor(float r, float g, float b);
};