#include "stdafx.h"

#include "Player.h"
#include "../../Math/Math.h"
#include "../Components/Transform.h"
#include "App/App.h"

static float last = 0.0f;

Player::Player(Vector3f position, Camera* camera, ParticleEmitter* emitter): _camera(camera), _emitter(emitter) {
	CreateComponent<Component::Transform>(Component::Transform({0, 0, 0}));

	camera->SetLookDir(Math::Normalize(position - camera->position));
}

void Player::Render(Graphics& context) {
	Component::Transform& transform = *GetComponent<Component::Transform>();

	context.RenderMesh(Meshes::CUBE, transform.CreateModelMatrix());
	_emitter->Render(context, Meshes::STAR);
}

void Player::Update(float deltaTime) {
	Component::Transform& transform = *GetComponent<Component::Transform>();

	_ProcessInput(transform);

	last += deltaTime;
	if (last >= 10.0f) {
		Vector3f randVel = {
			Math::RandomFloat(0.05f, 0.09f, true),
			Math::RandomFloat(0.05f, 0.09f, true),
			Math::RandomFloat(0.05f, 0.09f, true)
		};
		Vector3f randCol = {
			Math::RandomFloat(0.6f, 1.0f),
			Math::RandomFloat(0.6f, 1.0f),
			Math::RandomFloat(0.6f, 1.0f)
		};
		Vector3f randPos = {
			Math::RandomFloat(transform.position.x - 1.0f, transform.position.x + 1.0f),
			Math::RandomFloat(transform.position.y - 1.0f, transform.position.y + 1.0f),
			Math::RandomFloat(transform.position.z - 1.0f, transform.position.z + 1.0f)
		};
		_emitter->Create(randPos, randVel, Math::RandomFloat(100.0f, 1000.0f), Math::RandomFloat(0.3f, 0.7f), randCol);

		last = 0.0f;
	}
}

void Player::_ProcessInput(Component::Transform& transform) {
	Vector3f oldPos = transform.position;
	if (App::IsKeyPressed('W'))
		transform.position.z -= 0.5f;
	if (App::IsKeyPressed('S'))
		transform.position.z += 0.5f;
	if (App::IsKeyPressed('A'))
		transform.position.x -= 0.5f;
	if (App::IsKeyPressed('D'))
		transform.position.x += 0.5f;

	_camera->SetLookDir(Math::Normalize(transform.position - _camera->position));
	//_camera->Move(position - oldPos);
}