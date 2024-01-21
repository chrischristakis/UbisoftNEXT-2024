#pragma once

#include "Component.h"
#include "../../Math/Math.h"

namespace Component {

	class Rect: public Component {
	public:
		Vector2f position;
		Vector2f size;

		Rect();
		Rect(Vector2f position, Vector2f size);
	};

}