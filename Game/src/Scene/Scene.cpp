#include "stdafx.h"

#include "Scene.h"

#include "app\app.h"

#include "../Math/Math.h"

#include "Components/Box2D.h"
#include "Components/Physics.h"

Enemy *enemy;

Scene::Scene() {
	camera = new Camera(Vector3f(0, 0, 15));
	context = new Graphics(camera);
	emitter = new ParticleEmitter(300);
	bulletPool = new BulletPool(10);
	player = new Player({0, 0}, camera, emitter, bulletPool);
	deathbox = new Deathbox(0, 0, 15, 15);

	enemy = new Enemy({-6, -3}, {2, 1}, player, emitter, bulletPool);

	currentState = State::PREGAME;
}

Scene::~Scene() {
	delete context;
	delete camera;
	delete player;
	delete emitter;
	delete deathbox;

	delete bulletPool;
}

void Scene::Update(float deltaTime) {
	ProcessInput();

	if (currentState != State::GAME_OVER) {
		// Pre collision updates
		player->Update(deltaTime);
		emitter->Update(deltaTime);

		if (currentState != State::PREGAME) {
			bulletPool->Update(deltaTime);
			enemy->Update(deltaTime);
		}

		DoCollisions(deltaTime);
	}
	camera->Update();
	context->Update(); // Sorts everything that needs to be rendered
}

void Scene::ProcessInput() {
	if (App::IsKeyPressed('R'))
		Restart();
	if (App::IsKeyPressed(VK_LBUTTON) && currentState == State::PREGAME)
		currentState = State::PLAYING;
}

void Scene::Restart() {
	player->Reset();
	bulletPool->Clear();
	emitter->Clear();

	currentState = State::PREGAME;
}

void Scene::DoCollisions(float deltaTime) {
	Component::Transform& transform = *player->GetComponent<Component::Transform>();

	// Check if player has left the scene (Game over)
	if (!deathbox->InDeathbox(Vector2f(transform.position))) {
		currentState = State::GAME_OVER;
	}
}

void Scene::Render() {
	if (currentState == State::GAME_OVER) {
		App::Print(APP_VIRTUAL_WIDTH / 2.0f - 60, APP_VIRTUAL_HEIGHT / 2.0f + 70, "GAME OVER!", 1, 0, 0, GLUT_BITMAP_HELVETICA_18);
		App::Print(APP_VIRTUAL_WIDTH / 2.0f - 75, APP_VIRTUAL_HEIGHT / 2.0f + 40, "Press R to restart!", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
	}

	if (currentState == State::PREGAME) {
		App::Print(APP_VIRTUAL_WIDTH / 2.0f - 80, APP_VIRTUAL_HEIGHT / 2.0f + 120, 
			"Left click to shoot.", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
		App::Print(APP_VIRTUAL_WIDTH / 2.0f - 230, APP_VIRTUAL_HEIGHT / 2.0f + 90, 
			"The recoil from your ship's turret is the only way to move.", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
		App::Print(APP_VIRTUAL_WIDTH / 2.0f - 200, APP_VIRTUAL_HEIGHT / 2.0f + 60,
			"You have one shot every second. Make it count.", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
		App::Print(APP_VIRTUAL_WIDTH / 2.0f - 200, APP_VIRTUAL_HEIGHT / 2.0f + 30,
			"Stay within the red box. Or be lost to space...", 1, 0, 0, GLUT_BITMAP_HELVETICA_18);
	}

	player->Render(*context);
	deathbox->Render(*context);
	bulletPool->Render(*context);
	emitter->Render(*context);

	enemy->Render(*context);

	// Output rendered frame
	context->Flush();
}