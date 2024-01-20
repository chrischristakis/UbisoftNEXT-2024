#include "stdafx.h"
#include "App/AppSettings.h"
#include "App/App.h"
#include "../Math/MatrixTransform.h"
#include "../Math/Vector.h"
#include "Graphics.h"
#include "Primitive.h"
#include "../Constants.h"

Graphics::Graphics(Camera* camera): _camera(camera) {
	float aspectRatio = (float)APP_VIRTUAL_WIDTH / APP_VIRTUAL_HEIGHT;
	_projection = Math::Transform::Perspective(Constants::FOV, aspectRatio, Constants::NEAR_PLANE, Constants::FAR_PLANE);
}

void Graphics::RenderLine(Vector3f p1, Vector3f p2, Vector3f color) {
	Primitive line;
	line.PushVertex(p1);
	line.PushVertex(p2);

	Mesh lineMesh;
	lineMesh.push_back(line);

	_RenderMesh(lineMesh, Mat4x4::Identity(), false, color);
}

void Graphics::RenderMesh(const Mesh& mesh, const Mat4x4& model) {
	_RenderMesh(mesh, model, false, {0, 0, 0});
}

void Graphics::RenderMesh(const Mesh& mesh, const Mat4x4& model, const Vector3f& color) {
	_RenderMesh(mesh, model, true, color);
}

void Graphics::RenderMesh(const Mesh& mesh, const Mat4x4& model, float r, float g, float b) {
	_RenderMesh(mesh, model, true, { r, g, b });
}

void Graphics::_RenderMesh(const Mesh& mesh, const Mat4x4& model, bool useOverridenColor, const Vector3f& color) {
	
	for (const Primitive& prim : mesh) {
		PrimitiveAvgDepth processedPrim;

		float depthSum = 0.0f;
		bool clipped = false; // Immediate clipping for near and far planes
		int numClipped = 0; // Number of individual clipped vertices;

		// Transform each vertex in each primitive by the MVP matrix
		for (const Vertex& vert : prim.vertices) {

			Vector4f clipSpace = _projection * _camera->GetViewMatrix() * model * Vector4f(vert, 1.0f);

			// Before perspective division, we clip vertices. If -w <= z <= w, then valid.
			// As an intentional choice, I only immediately clip z values beyond the near or far plane.
			if (clipSpace.z > clipSpace.w || clipSpace.z < -clipSpace.w) {
				clipped = true;
				break;
			}

			// Track num of occurances of vertices that are off the screen.
			if( clipSpace.x > clipSpace.w || clipSpace.x < -clipSpace.w ||
				clipSpace.y > clipSpace.w || clipSpace.y < -clipSpace.w)
				numClipped++;

			Vertex clipSpaceVert{
				clipSpace.x / clipSpace.w,
				clipSpace.y / clipSpace.w,
				clipSpace.z / clipSpace.w,
			};

			// Viewport transform from NDC
			float scale_x = APP_VIRTUAL_WIDTH / 2.0f;
			float scale_y = APP_VIRTUAL_HEIGHT / 2.0f;

			Vertex screenSpaceVert{
				(clipSpaceVert.x + 1) * scale_x,
				(clipSpaceVert.y + 1) * scale_y,
				clipSpaceVert.z
			};

			depthSum += screenSpaceVert.z;

			processedPrim.PushVertex(screenSpaceVert);			
		}

		processedPrim.depth = depthSum / prim.vertices.size();

		if (!useOverridenColor)
			processedPrim.color = prim.color;
		else
			processedPrim.color = color;

		// Discard clipped primitives where 1 vert is beyond near and far plane (z), also discard
		// primitives where ALL vertices lie beyond the screen bounds (x and y)
		// This lets us have harsh z clipping the moment a vertex leaves the near and far plane,
		// (To prevent visual artefacts)
		// but more leeway for x and y clipping so that the primitive isnt clipped the moment
		// a single vertex leaves the frustum.
		if(!clipped && numClipped != prim.vertices.size())
			_primsToRender.emplace_back(processedPrim);
	}
}

void Graphics::SortOnDepth() {
	std::sort(_primsToRender.begin(), _primsToRender.end(), 
		[](const PrimitiveAvgDepth& a, const PrimitiveAvgDepth& b) {
			return a.depth > b.depth;
		}
	);
}

void Graphics::Update() {
	_camera->Update();
}
  
// Render everything we've collected and sorted.
void Graphics::Flush() {
	SortOnDepth();

	for (const PrimitiveAvgDepth& prim : _primsToRender) {
		for (int i = 0; i < prim.vertices.size(); i++) {

			const Vertex &v1 = prim.vertices[i];
			const Vertex &v2 = prim.vertices[(i+1) % prim.vertices.size()];

			App::DrawLine(v1.x, v1.y, v2.x, v2.y, prim.color.x, prim.color.y, prim.color.z);
		}
	}
	_primsToRender.clear();
}