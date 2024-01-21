#include "stdafx.h"

#include "Rect.h"

namespace Component {
	Rect::Rect() : Rect({ 0, 0 }, { 0, 0 }) { }
	Rect::Rect(Vector2f position, Vector2f size): position(position), size(size) { }
}