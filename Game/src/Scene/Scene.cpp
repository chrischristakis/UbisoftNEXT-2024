#include "stdafx.h"

#include "Scene.h"

#include "app\app.h"

#include "../Graphics/Graphics.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Primitive.h"
#include "../Graphics/Camera.h"
#include "../Graphics/ParticleEmitter.h"

#include "../Math/Math.h"
#include "GameObjects/Player.h"

Camera* camera;
Graphics* context;
Player* player;
ParticleEmitter* emitter;

Scene::Scene() {
	camera = new Camera(Vector3f(0, 14, 16));
	context = new Graphics(camera);
	emitter = new ParticleEmitter(100);
	player = new Player({0, 1, 0}, camera, emitter);
}

Scene::~Scene() {
	delete context;
	delete camera;
	delete player;
	delete emitter;
}

float i = 0;
void Scene::Update(float deltaTime) {
	player->Update(deltaTime);
	emitter->Update(deltaTime);

	camera->Update();
	context->Update(); // Sorts everything that needs to be rendered
}

void Scene::Render() {
	i += 0.5f;
	player->Render(*context);

	Mat4x4 model = Math::Transform::RotateX(Mat4x4::Identity(), 90.0f);
	model = Math::Transform::Scale(model, { 10.0f, 10.0f, 1.0f });
	context->RenderMesh(Meshes::QUAD, model, 1, 0, 0);
	
	model = Math::Transform::Translate(Mat4x4::Identity(), { 5.0f, 3.0f, -6.0f });
	model = Math::Transform::RotateY(model, i);
	model = Math::Transform::Scale(model, { 1.0f, 3.0f, 1.0f });
	context->RenderMesh(Meshes::CONE, model, 0, 1, 0);

	// Output rendered frame
	context->Flush();
}