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
#include "GameObjects/Platform.h"

#include "Components/Box2D.h"
#include "Components/Physics.h"

Camera* camera;
Graphics* context;
Player* player;
Platform* platform;
ParticleEmitter* emitter;

Scene::Scene() {
	camera = new Camera(Vector3f(0, 10, 10));
	context = new Graphics(camera);
	emitter = new ParticleEmitter(100);
	player = new Player({0, 1.0f, 0}, camera);
	platform = new Platform({ 0, 0, 0 }, {10.0f, 1.0f, 10.0f});
}

Scene::~Scene() {
	delete context;
	delete camera;
	delete player;
	delete emitter;
	delete platform;
}

void Scene::Update(float deltaTime) {
	
	// Pre collision updates
	player->Update(deltaTime);
	emitter->Update(deltaTime);

	DoCollisions();

	// Post collision updates
	player->LateUpdate(deltaTime);

	camera->Update();
	context->Update(); // Sorts everything that needs to be rendered
}

void Scene::DoCollisions() {
	/*
	Component::BoxCollider& c1 = *player->GetComponent<Component::BoxCollider>();
	Component::BoxCollider& c2 = *platform->GetComponent<Component::BoxCollider>();

	Component::Physics& physics = *player->GetComponent<Component::Physics>();
	Vector3f oldPos = c1.position;
	c1.position = c1.position + physics.velocity;

	if (c1.CollidesWith(c2)) {
		debug = "collided!";
	}
	else
		debug = "";
	*/
}

void Scene::Render() {
	player->Render(*context);
	platform->Render(*context);

	// Output rendered frame
	context->Flush();
}