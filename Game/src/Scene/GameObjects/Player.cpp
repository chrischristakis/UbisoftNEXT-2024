#include "stdafx.h"

#include "Player.h"
#include "../../Math/Math.h"
#include "../Components/Transform.h"
#include "../Components/Box2D.h"
#include "../Components/Physics.h"
#include "App/App.h"
#include "../../Constants.h"

Player::Player(Vector3f position, Camera* camera):
	_camera(camera), cursorPos({0, 0, 0}) 
{
	CreateComponent<Component::Transform>(Component::Transform(position, {0.5f, 1.0f, 0.5f}));
	CreateComponent<Component::Box2D>(Component::Box2D(Vector2f(position), Vector2f(0.5f, 1.0f)));
	CreateComponent<Component::Physics>(Component::Physics());

	camera->SetLookDir(Math::Normalize(position - camera->position));
}

void Player::Render(Graphics& context) {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	Component::Box2D& collider = *GetComponent<Component::Box2D>();

	Vector2f cp, cn;
	float t;
	if(collider.CollidesWithRay(Vector2f(transform.position),
								Vector2f(cursorPos - transform.position),
								cp, cn, t) && t < 1.0f)
		collider.Render(context, transform.position.z, {1, 1, 0});
	else
		collider.Render(context, transform.position.z);

	context.RenderMesh(Meshes::CUBE, transform.CreateModelMatrix());
	RenderAimLine(context);
}

void Player::Update(float deltaTime) {
	Component::Transform& transform = *GetComponent<Component::Transform>();

	ProcessInput();
}

void Player::LateUpdate(float deltaTime) {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	Component::Physics& physics = *GetComponent<Component::Physics>();
	Component::Box2D& collider = *GetComponent<Component::Box2D>();

	transform.position = transform.position + physics.velocity;

	collider.SyncPositionWithTransform(transform);

	_camera->SetLookDir(Math::Normalize(transform.position - _camera->position));
	UpdateCursorPos();
}

void Player::ProcessInput() {
	Component::Physics& physics = *GetComponent<Component::Physics>();

	Vector3f velocity;
	if (App::IsKeyPressed('A'))
		velocity.x -= 0.5f;
	if (App::IsKeyPressed('D'))
		velocity.x += 0.5f;

	physics.velocity = velocity;
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
	context.RenderMesh(Meshes::QUAD, model, { 1, 0, 0 });
	context.RenderMesh(lineMesh, Mat4x4::Identity(), {1, 0, 0});
}