#pragma once

#include "GameObject.h"
#include "../../Graphics/Graphics.h"

class Projectile: public GameObject {
public:
	float ttl;
	bool hostile;
	// Hostile determines whether the projectile will damage the player or not
	Projectile();
	Projectile(Vector2f position, Vector2f velocity, float ttl = 1.0f, bool hostile = true);

	void Update(float deltaTime) override;
	void Render(Graphics& context) override;
};