#pragma once

#include "app\app.h"

class Renderer {
public:
	void DrawSquare(float x, float y, float w, float h) {
		App::DrawLine(x, y, x+w, y);
		App::DrawLine(x, y+h, x+w, y+h);
		App::DrawLine(x, y, x, y+h);
		App::DrawLine(x+w, y, x+w, y+h);
	}
};