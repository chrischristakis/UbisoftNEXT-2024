#include "stdafx.h"

#include "Physics.h"

namespace Component {

	Physics::Physics() : Physics({ 0, 0, 0 }, { 0, 0, 0 }) { }
	Physics::Physics(Vector3f velocity, Vector3f acceleration): velocity(velocity), acceleration(acceleration) { }

}