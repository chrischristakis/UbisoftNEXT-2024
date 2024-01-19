#pragma once

#include "Component.h"
#include "../../Math/Math.h"

namespace Component {
	class Transform : public Component {
	public:
		Vector3f position;
		Vector3f scale;
		Vector3f rotation;

		Transform();
		Transform(Vector3f position, Vector3f scale = Vector3f{ 1, 1, 1 }, Vector3f rotation = Vector3f{0, 0, 0});

		Mat4x4 CreateModelMatrix();
	};
}