#pragma once

#include "Gameobject.h"
#include "../../Math/Math.h"

class Platform : public GameObject {
private:
public:
	Platform(Vector3f position, Vector3f scale=Vector3f{1, 1, 1});

	void Render(Graphics &g) override;
};