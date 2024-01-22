#include "stdafx.h"

#include "Player.h"
#include "../../Math/Math.h"
#include "../Components/Transform.h"
#include "../Components/Physics.h"
#include "../Components/Cooldown.h"
#include "../Components/CircleCollider.h"
#include "../Components/Health.h"
#include "App/App.h"
#include "../../Constants.h"

static const std::string FIRE_COOLDOWN = "fire";
static const std::string INVINCIBLE_COOLDOWN = "invincible";
static const float FIRE_RATE = 1.0f;
static const float INVINCIBLE_DURATION = 2.0f;
static const int INITIAL_HEALTH = 3;
static const float COLLIDER_RADIUS = 0.3f;
static const Vector3f MODEL_SCALE = { 0.5f, 1.0f, 0.5f };
static const Vector2f ACCELERATION = { 0.98f, 0.98f };

Player::Player(Vector2f position, Camera* camera, ParticleEmitter* emitter, BulletPool* bulletpool):
	_camera(camera), _bulletPool(bulletpool), _emitter(emitter), cursorPos({0, 0, 0})
{
	Component::Transform& transform = *CreateComponent<Component::Transform>(Component::Transform(Vector3f(position, 0.0f), MODEL_SCALE));
	CreateComponent<Component::Physics>(Component::Physics(Vector2f(), ACCELERATION));
	Component::Cooldown& cooldown = *CreateComponent<Component::Cooldown>(Component::Cooldown());
	CreateComponent<Component::CircleCollider>(Component::CircleCollider(position, COLLIDER_RADIUS));
	CreateComponent<Component::Health>(Component::Health(INITIAL_HEALTH));

	_camera->SetLookDir(Math::Normalize(transform.position - _camera->position));

	_initialPos = position;

	// Initialize cooldowns
	cooldown.CreateCooldown(FIRE_COOLDOWN, FIRE_RATE);
	cooldown.CreateCooldown(INVINCIBLE_COOLDOWN, INVINCIBLE_DURATION);
}

void Player::Reset() {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	Component::Physics& physics = *GetComponent<Component::Physics>();
	Component::Cooldown& cooldown = *GetComponent<Component::Cooldown>();
	Component::Health& health = *GetComponent<Component::Health>();

	cooldown.ResetAll();
	health.invincible = false;
	health.health = INITIAL_HEALTH;
	transform.position = Vector3f(_initialPos, 0.0f);
	physics.velocity = Vector2f(0, 0);
	_camera->SetLookDir(Math::Normalize(transform.position - _camera->position));
}


void Player::Render(Graphics& context) {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	Component::CircleCollider& collider = *GetComponent<Component::CircleCollider>();
	Component::Health& health = *GetComponent<Component::Health>();

	if (health.invincible)
		context.RenderMesh(Meshes::SHIP, transform.CreateModelMatrix(), {0, 0.5f, 0.5f});
	else
		context.RenderMesh(Meshes::SHIP, transform.CreateModelMatrix(), {0, 1, 1});

	collider.Render(context);
	RenderAimLine(context);

	RenderLives(context);
}

void Player::Update(float deltaTime) {
	
	UpdateCooldowns(deltaTime);
	ProcessInput();

	Component::Transform& transform = *GetComponent<Component::Transform>();
	Component::Physics& physics = *GetComponent<Component::Physics>();
	Component::CircleCollider& collider = *GetComponent<Component::CircleCollider>();

	physics.velocity = physics.velocity * physics.acceleration;
	transform.position = transform.position + Vector3f(physics.velocity, 0.0f) * deltaTime;
	collider.position = Vector2f(transform.position);

	_camera->SetLookDir(Math::Normalize(transform.position - _camera->position));
	UpdateCursorPos();
}

void Player::UpdateCooldowns(float deltaTime) {
	Component::Cooldown& cooldown = *GetComponent<Component::Cooldown>();

	cooldown.Update(deltaTime);

	Component::CooldownInfo& invincibleCd = cooldown.GetCooldown(INVINCIBLE_COOLDOWN);
	if (invincibleCd.Ready())
		GetComponent<Component::Health>()->invincible = false;
}

void Player::ProcessInput() {
	Component::Cooldown& cooldown = *GetComponent<Component::Cooldown>();
	Component::CooldownInfo& bulletCd = cooldown.GetCooldown(FIRE_COOLDOWN);

	if (App::IsKeyPressed(VK_LBUTTON) && bulletCd.Ready())
		FireBullet();
}

void Player::TriggerHit() {
	Component::Health& health = *GetComponent<Component::Health>();
	if (health.invincible)
		return;

	health.Hit(1);
	Component::Cooldown& cooldown = *GetComponent<Component::Cooldown>();
	Component::CooldownInfo& invincibleCd = cooldown.GetCooldown(INVINCIBLE_COOLDOWN);
	invincibleCd.Refresh();
	health.invincible = true;
}

void Player::FireBullet() {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	Component::Physics& physics = *GetComponent<Component::Physics>();

	Vector2f cursorDir = Math::Normalize(Vector2f(cursorPos) - Vector2f(transform.position));

	float bulletSpeed = 8.5f;
	Vector2f bulletVel = cursorDir * bulletSpeed;

	// knock back the ship with recoil
	physics.velocity = physics.velocity - bulletVel;

	// Spawn a bullet infront of the player
	Projectile p({ Vector2f(transform.position) }, bulletVel, 2.0f, false);
	_bulletPool->CreateBullet(p);

	// Spawn some particles
	for (int i = 0; i < 10; i++) {
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
		_emitter->Create(Particle{ randPos, &Meshes::STAR, randVel, 1.0f, 0.2f, {0.5f, 0.5f, 0.5f} });
	}

	// Reset the cooldown
	Component::Cooldown& cooldown = *GetComponent<Component::Cooldown>();
	Component::CooldownInfo& bulletCd = cooldown.GetCooldown(FIRE_COOLDOWN);
	bulletCd.Refresh();
}

void Player::UpdateCursorPos() {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	float mx, my;
	App::GetMousePos(mx, my);

	// Turn mouse into ndc
	Vector4f mndc = Vector4f(
		(mx / (float)APP_VIRTUAL_WIDTH * 2.0f) - 1.0f,
		(my / (float)APP_VIRTUAL_HEIGHT * 2.0f) - 1.0f,
		-1.0f,
		1.0f
	);

	// Turn ndc into view by inversing projection
	Vector4f mview;
	float aspect = (float)APP_VIRTUAL_WIDTH / (float)APP_VIRTUAL_HEIGHT;
	mview.x = mndc.x * aspect * tan(Constants::FOV / 2.0f);
	mview.y = mndc.y * tan(Constants::FOV / 2.0f);
	mview.z = - 1.0f / tan(Constants::FOV / 2.0f);
	mview.w = 1.0f;

	// Turn view into world
	Vector4f mworld = _camera->CalculateInverseLookAt() * mview;

	Vector3f rayPos(mworld.x, mworld.y, mworld.z);  // A positon
	Vector3f rayDir(rayPos - _camera->position); // Turn it into a direction.

	// Find where the ray intersects the z plane of the player using plane equation
	float magnitude = (transform.position.z - rayPos.z) / rayDir.z;
	cursorPos = _camera->position + rayDir * magnitude;
	cursorPos.z = transform.position.z;

	// Also update the rotation of the player
	Vector2f cursorDir = Math::Normalize(Vector2f(cursorPos) - Vector2f(transform.position));
	float num = Math::Dot(cursorDir, Vector2f(0, 1));
	float dem = Math::Magnitude(cursorDir);
	float angle = Math::RadToDeg(acos(num / dem));
	if (cursorDir.x > 0)
		angle = -angle;
	transform.rotation.z = angle;
}

void Player::RenderAimLine(Graphics& context) {
	const float AIM_LINE_LENGTH = 10.0f;

	Component::Transform& transform = *GetComponent<Component::Transform>();
	Vector3f cursorNormalized = Math::Normalize(cursorPos);

	Primitive line;
	line.PushVertex(cursorPos);
	line.PushVertex(transform.position);

	Mesh lineMesh;
	lineMesh.push_back(line);

	Mat4x4 model = Math::Transform::Translate(Mat4x4::Identity(), cursorPos);
	model = Math::Transform::Scale(model, { 0.1f, 0.1f, 0.1f });
	context.RenderMesh(Meshes::QUAD, model, { 1, 1, 1 });
	context.RenderMesh(lineMesh, Mat4x4::Identity(), {1, 1, 1});
}

static const float LIFE_SIZE = 0.1f;
static const float LIFE_SPACING = 0.2f;
void Player::RenderLives(Graphics& context) {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	Component::Health& health = *GetComponent<Component::Health>();

	for (int i = 0; i < health.health; i++) {
		Mat4x4 model = Math::Transform::Translate(Mat4x4::Identity(), transform.position);
		model = Math::Transform::Translate(model, {i * LIFE_SPACING - (LIFE_SPACING), -0.7f, 0.0f});
		model = Math::Transform::Scale(model, { LIFE_SIZE, LIFE_SIZE, 1.0f });
		context.RenderMesh(Meshes::QUAD, model, {0, 1, 0});
	}
}