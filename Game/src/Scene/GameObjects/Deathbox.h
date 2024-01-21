#pragma once

#include "GameObject.h"
#include "../Components/Transform.h"
#include "../../Graphics/Graphics.h"

class Deathbox : public GameObject {
public:
	Vector3f color;
	Deathbox(float x, float y, float w, float h);

	bool InDeathbox(Vector2f position);

	void Render(Graphics& context) override;
};