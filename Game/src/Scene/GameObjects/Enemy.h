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

	const float SHOOT_PLAYER_COOLDOWN = 1.0f;
	float _shootPlayerCooldownTimer;

	const float EMINATION_COOLDOWN = 0.05f;
	float _eminationCooldownTimer;

public:
	Enemy();
	Enemy(Vector2f position, Vector2f velocity, Player* player, ParticleEmitter* emitter, BulletPool* bulletpool);

	void Update(float deltaTime) override;
	void Render(Graphics& context) override;

	void ShootAtPlayer();
	void EminateParticle();
};