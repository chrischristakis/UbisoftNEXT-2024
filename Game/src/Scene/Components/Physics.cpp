#include "stdafx.h"

#include "Physics.h"

namespace Component {

	Physics::Physics() : Physics({ 0, 0 }, { 0, 0 }) { }
	Physics::Physics(Vector2f velocity, Vector2f acceleration): velocity(velocity), acceleration(acceleration) { }

}