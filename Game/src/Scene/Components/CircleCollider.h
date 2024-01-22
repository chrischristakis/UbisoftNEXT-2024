#pragma once

#include "Component.h"
#include "Transform.h"
#include "../../Math/Math.h"
#include "../../Graphics/Graphics.h"

namespace Component {

	class CircleCollider: public Component {
	public:
		Vector2f position;
		float radius;

		CircleCollider(Vector2f position, float radius);

		bool CollidesWith(const CircleCollider& other);
		void Render(Graphics& context);
	};

}