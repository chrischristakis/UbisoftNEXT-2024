#include "stdafx.h"

#include "Player.h"
#include "../Math/Math.h"
#include "../Graphics/Mesh.h"
#include "App/App.h"

#include "ParticleEmitter.h"

ParticleEmitter* pe;
float last = 0.0f;

Player::Player(Camera* camera) : _camera(camera), position(0, 1, 0) {
	camera->SetLookDir(Math::Normalize(position - camera->position));

	pe = new ParticleEmitter(50);
}

void Player::Render(Graphics& context) {
	Mat4x4 model = Transform::Translate(Mat4x4::Identity(), {position.x, position.y, position.z});
	context.RenderMesh(Meshes::CUBE, model);

	pe->Render(context, Meshes::QUAD);
}

void Player::Update(float deltaTime) {
	ProcessInput();

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
			Math::RandomFloat(position.x - 1.0f, position.x + 1.0f),
			Math::RandomFloat(position.y - 1.0f, position.y + 1.0f),
			Math::RandomFloat(position.z - 1.0f, position.z + 1.0f)
		};
		pe->Create(randPos, randVel, Math::RandomFloat(100.0f, 1000.0f), Math::RandomFloat(0.05f, 0.1f), randCol);

		last = 0.0f;
	}

	pe->Update(deltaTime);
}

void Player::ProcessInput() {
	Vector3f oldPos = position;
	if (App::IsKeyPressed('W'))
		position.z -= 0.5f;
	if (App::IsKeyPressed('S'))
		position.z += 0.5f;
	if (App::IsKeyPressed('A'))
		position.x -= 0.5f;
	if (App::IsKeyPressed('D'))
		position.x += 0.5f;

	_camera->SetLookDir(Math::Normalize(position - _camera->position));
	//_camera->Move(position - oldPos);
}