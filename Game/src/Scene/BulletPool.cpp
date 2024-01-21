#include "stdafx.h"

#include "BulletPool.h"
#include "Components/Transform.h"

BulletPool::BulletPool(int MAX_BULLETS) : MAX_BULLETS(MAX_BULLETS) {
	// Initialize pool with empty bullets
	for (int i = 0; i < MAX_BULLETS; i++)
		bullets.push_back(Projectile());
	_activeBullets = 0;
}

void BulletPool::Update(float deltaTime) {
	for (int i = 0; i < _activeBullets; i++) {
		if (bullets[i].ttl > 0.0f)
			bullets[i].Update(deltaTime);
		else
			_DestroyBullet(i);
	}
}

void BulletPool::Render(Graphics& context) {
	for (int i = 0; i < _activeBullets; i++)
		bullets[i].Render(context);
}

void BulletPool::CreateBullet(Projectile proj) {
	int index = _GetAvailableBullet();
	bullets[index] = proj;
}

int BulletPool::_GetAvailableBullet() {
	assert(_activeBullets < MAX_BULLETS && "Too many bullets!");
	return _activeBullets++;
}

void BulletPool::_DestroyBullet(int index) {
	assert(index >= 0 && index < _activeBullets && "Invalid index to destroy");
	
	Projectile lastActive = bullets[_activeBullets - 1];
	bullets[_activeBullets - 1] = bullets[index];
	bullets[index] = lastActive;
	_activeBullets--;
}

void BulletPool::Clear() {
	_activeBullets = 0;
}
