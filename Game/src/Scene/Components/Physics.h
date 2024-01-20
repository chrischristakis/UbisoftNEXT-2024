#pragma once

#include "Component.h"
#include "../../Math/Math.h"

namespace Component {

	class Physics: public Component{
	public:
		Vector3f velocity;
		Vector3f acceleration;

		Physics();
		Physics(Vector3f velocity, Vector3f acceleration);
	};

}