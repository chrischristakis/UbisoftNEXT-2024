#pragma once

#include "stdafx.h"
#include "../Graphics/Graphics.h"
#include "GameObjects/Projectile.h"

class BulletPool {	
public:
	const int MAX_BULLETS;
	std::vector<Projectile> bullets;
	unsigned int activeBullets = 0;

	int GetAvailableBullet();
	void DestroyBullet(int index);

	BulletPool(int MAX_BULLETS);

	void Update(float deltaTime);
	void Render(Graphics& context);

	void CreateBullet(Projectile proj);

	void Clear();
};