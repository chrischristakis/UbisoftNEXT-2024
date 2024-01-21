#include "stdafx.h"

#include "Enemy.h"
#include "../Components/Transform.h"
#include "../Components/Physics.h"
#include "Player.h"

Enemy::Enemy(): Enemy({0, 0}, {0, 0}, nullptr, nullptr, nullptr) { }
Enemy::Enemy(Vector2f position, Vector2f velocity, Player* player, ParticleEmitter* emitter, BulletPool* bulletpool): _player(player), _emitter(emitter), _bulletPool(bulletpool), _shootPlayerCooldownTimer(0), _eminationCooldownTimer(0) {
	Component::Transform& transform = *CreateComponent<Component::Transform>({ Vector3f(position, 0.0f), {0.5f, 0.5f, 0.5f}});
	CreateComponent<Component::Physics>({ velocity, {0, 0} });
}

void Enemy::Update(float deltaTime) {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	Component::Physics& physics = *GetComponent<Component::Physics>();

	if (_shootPlayerCooldownTimer > 0.0f)
		_shootPlayerCooldownTimer -= deltaTime;
	else
		ShootAtPlayer();

	if (_eminationCooldownTimer > 0.0f)
		_eminationCooldownTimer -= deltaTime;
	else
		EminateParticle();

	transform.position = transform.position + Vector3f(physics.velocity, 0.0f) * deltaTime;
}

void Enemy::EminateParticle() {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	float amplitude = 0.3f;
	Vector3f randPos{
			Math::RandomFloat(transform.position.x - amplitude, transform.position.x + amplitude),
			Math::RandomFloat(transform.position.y - amplitude, transform.position.y + amplitude),
			Math::RandomFloat(transform.position.z - amplitude, transform.position.z + amplitude),
	};
	Vector3f randVel{
		Math::RandomFloat(-0.5f, 0.5f),
		Math::RandomFloat(-0.5f, 0.5f),
		Math::RandomFloat(-0.5f, 0.5f),
	};

	Particle particle{
		randPos,
		&Meshes::QUAD,
		randVel,
		0.8f,
		0.05f,
		{1, 0, 1}
	};
	_emitter->Create(particle);

	_eminationCooldownTimer = EMINATION_COOLDOWN;
}

void Enemy::ShootAtPlayer() {

	Component::Transform& enemyTransform = *GetComponent<Component::Transform>();
	Component::Transform& playerTransform = *_player->GetComponent<Component::Transform>();

	Vector2f direction = Math::Normalize(Vector2f(playerTransform.position - enemyTransform.position));

	float bulletSpeed = 5.0f;
	Projectile bullet{
		enemyTransform.position,
		direction * bulletSpeed,
		3.0f,
		true
	};

	_bulletPool->CreateBullet(bullet);

	_shootPlayerCooldownTimer = SHOOT_PLAYER_COOLDOWN;
}

void Enemy::Render(Graphics& context) {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	Mat4x4 model = Math::Transform::Translate(Mat4x4::Identity(), transform.position);
	model = Math::Transform::Scale(model, transform.scale);
	context.RenderUnclippedMesh(Meshes::QUAD, model, { 1, 0, 1 });
	model = Math::Transform::Translate(Mat4x4::Identity(), transform.position);
	model = Math::Transform::Scale(model, transform.scale * 0.55f);
	context.RenderMesh(Meshes::QUAD, model, {1, 0, 1});
}