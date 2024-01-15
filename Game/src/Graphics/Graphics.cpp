#include "stdafx.h"
#include "App/AppSettings.h"
#include "App/App.h"
#include "../Math/Matrix_Transform.h"
#include "../Math/Vector.h"
#include "Graphics.h"
#include "Primitive.h"

Graphics::Graphics() {
	float aspectRatio = (float)APP_VIRTUAL_WIDTH / APP_VIRTUAL_HEIGHT;
	_projection = Transform::Perspective(45.0f, aspectRatio, _near, _far);
	_cam = new Camera(Vector3f(0, 0, 30));
}

void Graphics::RenderMesh(const Mesh& mesh, const Mat4x4& model) {
	
	for (const Primitive& prim : mesh) {
		PrimitiveAvgDepth processedPrim;

		// Transform each polygon by the MVP matrix
		float depthSum = 0.0f;
		bool clipped = false;
		for (const Vertex& vert : prim.vertices) {

			Vector4f clipSpace = _projection * _cam->GetViewMatrix() * model * Vector4f(vert, 1.0f);

			// Before perspective division, we clip vertices. If -w <= x, y, z <= w, then valid.
			// As an intentional choice, I divide the clip space x and y by some value, which gives some leeway out of
			// the frustum before clipping. Otherwise, if a single vertex of the primitive is outside the frustum, then
			// it will clip the entire thing. I do not do this for z since the leeway could result in graphics artifacts
			// since the primitive might render upside down, as it extends beyond the near plane.
			if (clipSpace.x*0.7f > clipSpace.w || clipSpace.x*0.7f < -clipSpace.w ||
				clipSpace.y*0.7f > clipSpace.w || clipSpace.y*0.7f < -clipSpace.w ||
				clipSpace.z > clipSpace.w || clipSpace.z < -clipSpace.w) {
				clipped = true;
				break;
			}

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
		processedPrim.color = prim.color;

		// Discard clipped primitives
		if(!clipped)
			_processedPrims.emplace_back(processedPrim);
	}
}

void Graphics::SortOnDepth() {
	std::sort(_processedPrims.begin(), _processedPrims.end(), 
		[](const PrimitiveAvgDepth& a, const PrimitiveAvgDepth& b) {
			return a.depth > b.depth;
		}
	);
}

void Graphics::Update() {
	_cam->Update();
}
  
// Render everything we've collected and sorted.
void Graphics::Flush() {
	SortOnDepth();

	App::Print(10.0f, 600.0f, std::to_string(_processedPrims.size()).c_str());
	int j = 0;
	for (const PrimitiveAvgDepth& prim : _processedPrims) {
		for (int i = 0; i < prim.vertices.size(); i++) {

			const Vertex &v1 = prim.vertices[i];
			const Vertex &v2 = prim.vertices[(i+1) % prim.vertices.size()];

			App::DrawLine(v1.x, v1.y, v2.x, v2.y, prim.color.x, prim.color.y, prim.color.z);
		}
	}
	_processedPrims.clear();
}