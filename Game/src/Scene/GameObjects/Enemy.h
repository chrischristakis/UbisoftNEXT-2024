#pragma once

#include "GameObject.h"
#include "Player.h"
#include "../BulletPool.h"
#include "../../Graphics/ParticleEmitter.h"

class Enemy : public GameObject {
private:
	Player* _player;
	BulletPool* _bulletPool;
	ParticleEmitter* _emitter;

public:
	Enemy();
	Enemy(Vector2f position, Vector2f velocity, Player* player, ParticleEmitter* emitter, BulletPool* bulletpool);

	void Update(float deltaTime) override;
	void UpdateCooldowns(float deltaTime);
	void Render(Graphics& context) override;

	void Explode(); // just A particle effect

	void ShootAtPlayer();
	void EminateParticle();
};