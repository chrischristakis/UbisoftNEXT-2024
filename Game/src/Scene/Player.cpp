#include "stdafx.h"

#include "Player.h"
#include "../Math/Math.h"
#include "../Graphics/Mesh.h"
#include "App/App.h"

Player::Player(Camera* camera) : _camera(camera), position(0, 1, 0) {
	camera->SetLookDir(Math::Normalize(position - camera->position));
}

void Player::Render(Graphics& context) {
	Mat4x4 model = Transform::Translate(Mat4x4::Identity(), {position.x, position.y, position.z});
	context.RenderMesh(Meshes::CUBE, model);

	Vector3f v1(2, -3, 1);
	Vector3f v2(-2, 1, 1);

	App::Print(10, 200, Math::Cross(v1, v2).to_string().c_str());
}

void Player::Update() {
	ProcessInput();
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