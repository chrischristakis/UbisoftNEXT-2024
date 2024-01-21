#pragma once

#include "Component.h"
#include "../../Math/Math.h"

namespace Component {

	class Physics: public Component{
	public:
		Vector2f velocity;
		Vector2f acceleration;

		Physics();
		Physics(Vector2f velocity, Vector2f acceleration = Vector2f(0, 0));
	};

}