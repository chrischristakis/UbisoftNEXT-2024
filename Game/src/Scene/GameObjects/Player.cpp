#include "stdafx.h"

#include "Player.h"
#include "../../Math/Math.h"
#include "../Components/Transform.h"
#include "../Components/Physics.h"
#include "App/App.h"
#include "../../Constants.h"

Player::Player(Vector2f position, Camera* camera, ParticleEmitter* emitter, BulletPool* bulletpool):
	_camera(camera), _bulletPool(bulletpool), _emitter(emitter), cursorPos({0, 0, 0}), _bulletCooldownTimer(0)
{
	Component::Transform& transform = *CreateComponent<Component::Transform>(Component::Transform(Vector3f(position, 1.0f), {0.5f, 1.0f, 0.5f}));
	CreateComponent<Component::Physics>(Component::Physics(Vector2f(), Vector2f(0.98f, 0.98f)));

	_camera->SetLookDir(Math::Normalize(transform.position - _camera->position));

	_initialPos = position;
}

void Player::Reset() {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	Component::Physics& physics = *GetComponent<Component::Physics>();

	_bulletCooldownTimer = 0;
	transform.position = Vector3f(_initialPos, 0.0f);
	physics.velocity = Vector2f(0, 0);
	_camera->SetLookDir(Math::Normalize(transform.position - _camera->position));
}


void Player::Render(Graphics& context) {
	Component::Transform& transform = *GetComponent<Component::Transform>();

	context.RenderMesh(Meshes::SHIP, transform.CreateModelMatrix(), {0, 1, 1});
	RenderAimLine(context);
}

void Player::Update(float deltaTime) {
	ProcessInput();

	if(_bulletCooldownTimer > 0.0f)
		_bulletCooldownTimer -= deltaTime;

	Component::Transform& transform = *GetComponent<Component::Transform>();
	Component::Physics& physics = *GetComponent<Component::Physics>();

	physics.velocity = physics.velocity * physics.acceleration;
	transform.position = transform.position + Vector3f(physics.velocity, 0.0f) * deltaTime;

	_camera->SetLookDir(Math::Normalize(transform.position - _camera->position));
	UpdateCursorPos();
}

void Player::ProcessInput() {
	if (App::IsKeyPressed(VK_LBUTTON) && _bulletCooldownTimer <= 0.0f)
		FireBullet();
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
	_bulletCooldownTimer = BULLET_COOLDOWN;
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
	float dem = Math::Distance(cursorDir);
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