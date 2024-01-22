#include "stdafx.h"

#include "Enemy.h"
#include "../Components/Transform.h"
#include "../Components/Physics.h"
#include "../Components/Cooldown.h"
#include "../Components/CircleCollider.h"
#include "Player.h"

static const std::string SHOOT_PLAYER_COOLDOWN = "shootplayer";
static const std::string EMINATION_COOLDOWN = "emination";
static const float SHOOT_PLAYER_RATE = 2.0f;
static const float EMINATION_RATE = 0.06f;
static const float BULLET_SPEED = 3.5f;
static const float COLLISION_RADIUS = 0.5f;
static const Vector3f MODEL_SCALE{0.7f, 0.7f, 0.7f};
static const float EXPLOSION_SPEED = 3.5f;
static const float EMINATION_SPEED = 1.5f;
static unsigned int EXPLOSION_AMOUNT = 20;

Enemy::Enemy(): Enemy({0, 0}, {0, 0}, nullptr, nullptr, nullptr) { }

Enemy::Enemy(Vector2f position, Vector2f velocity, Player* player, ParticleEmitter* emitter, BulletPool* bulletpool): _player(player), _emitter(emitter), _bulletPool(bulletpool) {
	Component::Transform& transform = *CreateComponent<Component::Transform>({ Vector3f(position, 0.0f), MODEL_SCALE});
	CreateComponent<Component::Physics>({ velocity, {0, 0} });
	Component::Cooldown& cooldown = *CreateComponent<Component::Cooldown>(Component::Cooldown());
	Component::CircleCollider& collider = *CreateComponent<Component::CircleCollider>(Component::CircleCollider(position, COLLISION_RADIUS));

	cooldown.CreateCooldown(SHOOT_PLAYER_COOLDOWN, SHOOT_PLAYER_RATE);
	cooldown.CreateCooldown(EMINATION_COOLDOWN, EMINATION_RATE);
}

void Enemy::Update(float deltaTime) {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	Component::Physics& physics = *GetComponent<Component::Physics>();
	Component::CircleCollider& collider = *GetComponent<Component::CircleCollider>();

	UpdateCooldowns(deltaTime);

	transform.position = transform.position + Vector3f(physics.velocity, 0.0f) * deltaTime;
	collider.position = Vector2f(transform.position);
}

void Enemy::UpdateCooldowns(float deltaTime) {
	Component::Cooldown& cooldown = *GetComponent<Component::Cooldown>();
	Component::CooldownInfo& shootPlayerCd = cooldown.GetCooldown(SHOOT_PLAYER_COOLDOWN);
	Component::CooldownInfo& eminateCd = cooldown.GetCooldown(EMINATION_COOLDOWN);

	cooldown.Update(deltaTime);

	if (shootPlayerCd.Ready())
		ShootAtPlayer();
	if (eminateCd.Ready())
		EminateParticle();
}

void Enemy::Explode() {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	for (int i = 0; i < EXPLOSION_AMOUNT; i++) {
		Vector3f randVel{
			Math::RandomFloat(-EXPLOSION_SPEED, EXPLOSION_SPEED),
			Math::RandomFloat(-EXPLOSION_SPEED, EXPLOSION_SPEED),
			Math::RandomFloat(-EXPLOSION_SPEED, EXPLOSION_SPEED),
		};

		Particle particle{
			transform.position,
			&Meshes::STAR,
			randVel,
			0.5f,
			Math::RandomFloat(0.1f, 0.3f),
			{1, 0, 1}
		};
		_emitter->Create(particle);
	}
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
		Math::RandomFloat(-EMINATION_SPEED, EMINATION_SPEED),
		Math::RandomFloat(-EMINATION_SPEED, EMINATION_SPEED),
		Math::RandomFloat(-EMINATION_SPEED, EMINATION_SPEED),
	};

	Particle particle{
		randPos,
		&Meshes::QUAD,
		randVel,
		0.8f,
		0.3f,
		{1, 0, 1}
	};
	_emitter->Create(particle);

	Component::Cooldown& cooldown = *GetComponent<Component::Cooldown>();
	Component::CooldownInfo& eminateCd = cooldown.GetCooldown(EMINATION_COOLDOWN);
	eminateCd.Refresh();
}

void Enemy::ShootAtPlayer() {

	Component::Transform& enemyTransform = *GetComponent<Component::Transform>();
	Component::Transform& playerTransform = *_player->GetComponent<Component::Transform>();

	Vector2f direction = Math::Normalize(Vector2f(playerTransform.position - enemyTransform.position));

	Projectile bullet{
		enemyTransform.position,
		direction * BULLET_SPEED,
		5.0f,
		true
	};

	_bulletPool->CreateBullet(bullet);

	Component::Cooldown& cooldown = *GetComponent<Component::Cooldown>();
	Component::CooldownInfo& shootPlayerCd = cooldown.GetCooldown(SHOOT_PLAYER_COOLDOWN);
	shootPlayerCd.Refresh();
}

void Enemy::Render(Graphics& context) {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	Component::CircleCollider& collider = *GetComponent<Component::CircleCollider>();

	Mat4x4 model = Math::Transform::Translate(Mat4x4::Identity(), transform.position);
	model = Math::Transform::Scale(model, transform.scale);
	context.RenderUnclippedMesh(Meshes::CUBE, model, { 1, 0, 1 });

	model = Math::Transform::Translate(Mat4x4::Identity(), transform.position);
	model = Math::Transform::Scale(model, transform.scale * 0.55f);
	context.RenderMesh(Meshes::CUBE, model, {1, 0, 0});

	collider.Render(context);
}