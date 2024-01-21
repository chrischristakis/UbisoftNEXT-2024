#pragma once

#include "Component.h"
#include "Transform.h"
#include "../../Math/Math.h"
#include "../../Graphics/Graphics.h"

namespace Component {

	class Box2D : public Component {
	public:
		Vector2f position;
		Vector2f size;
		Box2D(Vector2f position, Vector2f size);

		void SyncPositionWithTransform(Transform& transform);

		bool CollidesWithRay(const Vector2f& rayOrigin, const Vector2f& rayDir, 
							 Vector2f& contactPoint, Vector2f& contactNormal, float& timeToHitNear) const;

		// Draw colliders on a plane defined by z
		void Render(Graphics& context, float z, Vector3f color = {0, 1, 1});
	};

}