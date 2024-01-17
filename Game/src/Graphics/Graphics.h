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

	void SortOnDepth(); // Primitives with a higher depth value get drawn first.

	void _RenderMesh(const Mesh& m, const Mat4x4& model, bool useOverridenColor, const Vector3f& color);
public:
	Graphics(Camera* camera);

	// Rendering a mesh simply applies the MVP to the primitive, then
	// it stores them in a vector so we can order them due to the
	// lack of a depth buffer, so we render furthest -> closest.
	void RenderMesh(const Mesh& m, const Mat4x4& model);
	void RenderMesh(const Mesh& mesh, const Mat4x4& model, const Vector3f& color);
	void RenderMesh(const Mesh& mesh, const Mat4x4& model, float r, float g, float b);

	void Update();
	void Flush();
};