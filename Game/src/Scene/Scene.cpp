#include "stdafx.h"

#include "Scene.h"

#include "app\app.h"

#include "../Graphics/Graphics.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Primitive.h"
#include "../Math/Math.h"
#include "../Graphics/Camera.h"
#include "Player.h"

Camera* camera;
Graphics* context;
Player* player;

Scene::Scene() {
	camera = new Camera(Vector3f(0, 14, 16));
	context = new Graphics(camera);
	player = new Player(camera);
}

Scene::~Scene() {
	delete context;
	delete camera;
}

float x = 0, z = 0;
float i = 0;
void Scene::Update(float deltaTime) {
	player->Update(deltaTime);

	context->Update();
}

void Scene::Render() {
	i += 0.5f;
	player->Render(*context);

	Mat4x4 model = Transform::RotateX(Mat4x4::Identity(), 90.0f);
	model = Transform::Scale(model, { 10.0f, 10.0f, 1.0f });
	context->RenderMesh(Meshes::QUAD, model, 1, 0, 0);
	

	model = Transform::Translate(Mat4x4::Identity(), { 5.0f, 3.0f, -6.0f });
	model = Transform::RotateY(model, i);
	model = Transform::Scale(model, { 1.0f, 3.0f, 1.0f });
	context->RenderMesh(Meshes::CONE, model, 0, 1, 0);

	context->Flush();
}