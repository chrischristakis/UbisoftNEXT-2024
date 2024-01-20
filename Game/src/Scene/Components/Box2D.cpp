#include "stdafx.h"

#include "Box2D.h"
#include "../../Graphics/Graphics.h"

namespace Component {

	Box2D::Box2D(Vector2f position, Vector2f size) {
		this->position.x = position.x;
		this->position.y = position.y;
		this->size = size;
	}

	void Box2D::SyncPositionWithTransform(Transform& transform) {
		this->position.x = transform.position.x - size.x / 2.0f;
		this->position.y = transform.position.y - size.y / 2.0f + 5.0f;
	}

	bool Box2D::CollidesWithRay(const Vector2f& rayOrigin, const Vector2f& rayDir,
		Vector2f& contactPoint, Vector2f& contactNormal, float& timeToHitNear) 
	{
		// The 'time' for the paramatized vector until the collision with the Box2D
		Vector2f tNear = (position - rayOrigin) / rayDir;
		Vector2f tFar = (position + size - rayOrigin) / rayDir;
		
		// Ensure that the near and far axis are closest to the ray origin
		if (tNear.x > tFar.x) {
			float temp = tNear.x;
			tNear.x = tFar.x;
			tFar.x = temp;
		}
		if (tNear.y > tFar.y) {
			float temp = tNear.y;
			tNear.y = tFar.y;
			tFar.y = temp;
		}

		// If the collision rules aren't met, then no collision has occurred
		if (tNear.x > tFar.y || tNear.y > tFar.x)
			return false;

		// We use the tx or ty component of the collision, whichever one is closer.
		timeToHitNear = std::max(tNear.x, tNear.y);
		float timeToHitFar = std::min(tFar.x, tFar.y);

		// If this box is behind the ray direction, not a valid collision.
		// In these cases, our t value will be negative.
		if (timeToHitFar < 0) 
			return false;

		contactPoint = (rayOrigin + timeToHitNear) * rayDir;

		// Return the proper normal depending on our 
		if (tNear.x > tNear.y)
			if (rayDir.x < 0)
				contactNormal = { 1, 0 };
			else
				contactNormal = { -1, 0 };
		else if (tNear.x < tNear.y)
			if (rayDir.y < 0)
				contactNormal = { 0, 1 };
			else
				contactNormal = { 0, -1 };

		return true;
	}

	void Box2D::Render(Graphics& context, float z, Vector3f color) {
		Vertex v1{position.x, position.y, z};
		Vertex v2{position.x+size.x, position.y, z};
		Vertex v3{position.x+size.x, position.y+size.y, z};
		Vertex v4{position.x, position.y+size.y, z};

		Primitive front{ v1, v2, v3, v4 };

		Mesh mesh { front };

		context.RenderMesh(mesh, Mat4x4::Identity(), color);
	}

}