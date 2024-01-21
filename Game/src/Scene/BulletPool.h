#pragma once

#include "stdafx.h"
#include "../Graphics/Graphics.h"
#include "GameObjects/Projectile.h"

class BulletPool {
private:
	const int MAX_BULLETS;
	std::vector<Projectile> bullets;
	unsigned int _activeBullets = 0;

	int _GetAvailableBullet();
	void _DestroyBullet(int index);
public:
	BulletPool(int MAX_BULLETS);

	void Update(float deltaTime);
	void Render(Graphics& context);

	void CreateBullet(Projectile proj);

	void Clear();
};