#include "stdafx.h"

#include "BulletPool.h"
#include "Components/Transform.h"
#include "Components/CircleCollider.h"

BulletPool::BulletPool(int MAX_BULLETS) : MAX_BULLETS(MAX_BULLETS) {
	// Initialize pool with empty bullets
	for (int i = 0; i < MAX_BULLETS; i++)
		bullets.push_back(Projectile());
	activeBullets = 0;
}

void BulletPool::Update(float deltaTime) {
	std::vector<int> toRemove;
	for (int i = 0; i < activeBullets; i++) {
		if (bullets[i].ttl > 0.0f)
			bullets[i].Update(deltaTime);
		else
			toRemove.push_back(i);
	}

	for (int i : toRemove)
		DestroyBullet(i);
}

void BulletPool::Render(Graphics& context) {
	for (int i = 0; i < activeBullets; i++)
		bullets[i].Render(context);
}

void BulletPool::CreateBullet(Projectile proj) {
	int index = GetAvailableBullet();
	if (index == -1)
		return;
	bullets[index] = proj;
}

int BulletPool::GetAvailableBullet() {
	if (activeBullets >= MAX_BULLETS)
		return -1;
	return activeBullets++;
}

void BulletPool::DestroyBullet(int index) {
	assert(index >= 0 && index < activeBullets && "Invalid index to destroy");
	
	Projectile lastActive = bullets[activeBullets - 1];
	bullets[activeBullets - 1] = bullets[index];
	bullets[index] = lastActive;
	activeBullets--;
}

void BulletPool::Clear() {
	activeBullets = 0;
}
