#include "stdafx.h"

#include "Scene.h"
#include "app/app.h"

#include "../Math/Math.h"
#include "Components/Physics.h"
#include "Components/CircleCollider.h"
#include "Components/Health.h"

static const std::string ENEMY_SPAWN_COOLDOWN = "enemyspawn";
static const std::string ENEMY_SPAWN_SPEED_UP_COOLDOWN = "enemyspawnspeedup";
static const std::string ADD_POINTS_COOLDOWN = "addpointcooldown";
static const float ENEMY_SPAWN_RATE = 5.0f; // In seconds
static const float ENEMY_SPAWN_RATE_MIN = 1.0f; // Minimum enemy spawn amount
static const float ENEMY_SPAWN_RATE_SPEED_UP = 6.0f; // Increase spawn rates this often
static const float ENEMY_SPAWN_RATE_INCREMENT = 0.5f; // How much to decrease spawn rates by
static const float GAME_START_ENEMY_DELAY = 2.5f;

static const float ADD_POINT_AMOUNT = 1.0f;
static const float ADD_POINT_RATE = 1.0f;

static const float DEATHBOX_SIZE = 18.0f;

Scene::Scene() {
	score = 0;
	camera = new Camera(Vector3f(0, 0, 22));
	context = new Graphics(camera);
	emitter = new ParticleEmitter(200);
	bulletPool = new BulletPool(70);
	enemyPool = new EnemyPool(10);
	player = new Player({0, 0}, camera, emitter, bulletPool);
	deathbox = new Deathbox(0, 0, DEATHBOX_SIZE, DEATHBOX_SIZE);

	currentState = State::PREGAME;

	_cooldowns.CreateCooldown(ENEMY_SPAWN_COOLDOWN, ENEMY_SPAWN_RATE, GAME_START_ENEMY_DELAY);
	_cooldowns.CreateCooldown(ENEMY_SPAWN_SPEED_UP_COOLDOWN, ENEMY_SPAWN_RATE_SPEED_UP, ENEMY_SPAWN_RATE_SPEED_UP);
	_cooldowns.CreateCooldown(ADD_POINTS_COOLDOWN, ADD_POINT_RATE, ADD_POINT_RATE);
}

Scene::~Scene() {
	delete context;
	delete camera;
	delete player;
	delete emitter;
	delete deathbox;
	delete bulletPool;
	delete enemyPool;
}

void Scene::Update(float deltaTime) {
	ProcessInput();

	if (currentState != State::GAME_OVER) {

		// Pre collision updates
		player->Update(deltaTime);
		emitter->Update(deltaTime);

		if (currentState != State::PREGAME) {
			bulletPool->Update(deltaTime);
			enemyPool->Update(deltaTime);
			_cooldowns.Update(deltaTime);
		}

		Component::CooldownInfo& spawnCooldown = _cooldowns.GetCooldown(ENEMY_SPAWN_COOLDOWN);
		Component::CooldownInfo& increaseSpawnCooldown = _cooldowns.GetCooldown(ENEMY_SPAWN_SPEED_UP_COOLDOWN);
		Component::CooldownInfo& addPointCooldown = _cooldowns.GetCooldown(ADD_POINTS_COOLDOWN);
		if (spawnCooldown.Ready())
			SpawnEnemy();
		if (increaseSpawnCooldown.Ready()) {
			// Speed up the enemy spawn rates occasionally for difficulty
			spawnCooldown.cooldown = max(ENEMY_SPAWN_RATE_MIN, spawnCooldown.cooldown - ENEMY_SPAWN_RATE_INCREMENT);
			increaseSpawnCooldown.Refresh();
		}
		if (addPointCooldown.Ready()) {
			score += ADD_POINT_AMOUNT;
			addPointCooldown.Refresh();
		}

		DoCollisions(deltaTime);

		// If player is dead, end the game
		if (player->GetComponent<Component::Health>()->IsDead())
			currentState = State::GAME_OVER;
	}

	camera->Update();
	context->Update(); // Sorts everything that needs to be rendered
}

void Scene::SpawnEnemy() {
	// Spawn the enemy either along the y or x directions randomly.

	Vector2f randPos;
	Vector2f vel;

	if (Math::RandomBool()) {
		// Spawn on x axis
		randPos.x = Math::RandomFloat(-DEATHBOX_SIZE / 2.0f, DEATHBOX_SIZE / 2.0f);
		int sign = (Math::RandomBool() ? -1 : 1);
		randPos.y = (DEATHBOX_SIZE / 2.0f + 3.0f) * sign;

		vel = { Math::RandomFloat(-1.0f, 1.0f), Math::RandomFloat(1.5f, 5.0f) * sign * -1 };
	}
	else {
		// spawn on y axis
		int sign = (Math::RandomBool() ? -1 : 1);
		randPos.x = (DEATHBOX_SIZE / 2.0f + 3.0f) * sign;
		randPos.y = Math::RandomFloat(-DEATHBOX_SIZE / 2.0f, DEATHBOX_SIZE / 2.0f);

		vel = { Math::RandomFloat(1.5f, 5.0f) * sign * -1, Math::RandomFloat(-1.0f, 1.0f), };
	}

	enemyPool->CreateEnemy(Enemy{
		randPos,
		vel,
		player,
		emitter,
		bulletPool
	});

	_cooldowns.GetCooldown(ENEMY_SPAWN_COOLDOWN).Refresh();
}

void Scene::ProcessInput() {
	if (App::IsKeyPressed('R'))
		Restart();
	if (App::IsKeyPressed(VK_LBUTTON) && currentState == State::PREGAME)
		currentState = State::PLAYING;
}

void Scene::Restart() {
	score = 0;
	player->Reset();
	bulletPool->Clear();
	enemyPool->Clear();
	emitter->Clear();
	_cooldowns.ResetAll();
	_cooldowns.GetCooldown(ENEMY_SPAWN_COOLDOWN).timer = GAME_START_ENEMY_DELAY;
	_cooldowns.GetCooldown(ENEMY_SPAWN_COOLDOWN).cooldown = ENEMY_SPAWN_RATE;
	_cooldowns.GetCooldown(ENEMY_SPAWN_SPEED_UP_COOLDOWN).Refresh();
	_cooldowns.GetCooldown(ADD_POINTS_COOLDOWN).Refresh();

	currentState = State::PREGAME;
}

void Scene::DoCollisions(float deltaTime) {
	Component::Transform& playerTransform = *player->GetComponent<Component::Transform>();

	// Check if player has left the scene (Game over)
	if (!deathbox->InDeathbox(Vector2f(playerTransform.position))) {
		currentState = State::GAME_OVER;
	}

	Component::CircleCollider& playerCollider = *player->GetComponent<Component::CircleCollider>();
	Component::Health& playerHealth = *player->GetComponent<Component::Health>();
	std::vector<int> toRemove;

	// handle Player - Bullet collisions
	for (int i = 0; i < bulletPool->activeBullets; i++) {
		Projectile& bullet = bulletPool->bullets[i];

		// Do not check player bullets against the player.
		if (!bullet.hostile) continue;

		Component::CircleCollider& bulletCollider = *bullet.GetComponent<Component::CircleCollider>();

		if (!playerHealth.invincible && playerCollider.CollidesWith(bulletCollider)) {
			toRemove.push_back(i);
			player->TriggerHit();
		}
		
	}

	for (int i : toRemove)
		bulletPool->DestroyBullet(i);
	toRemove.clear();

	// handle Enemy - Bullet collisions
	for (int i = 0; i < bulletPool->activeBullets; i++) {

		Projectile& bullet = bulletPool->bullets[i];
		if (bullet.hostile) continue;  // Ignore enemy-fired bullets

		Component::CircleCollider& bulletCollider = *bullet.GetComponent<Component::CircleCollider>();

		for (int j = 0; j < enemyPool->activeEnemies; j++) {
			Enemy& enemy = enemyPool->enemies[j];
			Component::CircleCollider& enemyCollider = *enemy.GetComponent<Component::CircleCollider>();

			if (enemyCollider.CollidesWith(bulletCollider)) {
				score += 5;
				enemy.Explode();
				toRemove.push_back(j);
			}
		}
	}

	for (int i : toRemove)
		enemyPool->DestroyEnemy(i);
	toRemove.clear();

}

void Scene::Render() {
	if (currentState == State::GAME_OVER) {
		App::Print(APP_VIRTUAL_WIDTH / 2.0f - 60, APP_VIRTUAL_HEIGHT / 2.0f + 70, "GAME OVER!", 1, 0, 0, GLUT_BITMAP_HELVETICA_18);
		App::Print(APP_VIRTUAL_WIDTH / 2.0f - 75, APP_VIRTUAL_HEIGHT / 2.0f + 40, "Press R to restart!", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
	}

	if (currentState == State::PREGAME) {
		App::Print(APP_VIRTUAL_WIDTH / 2.0f - 240, APP_VIRTUAL_HEIGHT / 2.0f + 120, 
			"Left click to shoot, increase your score by hitting the aliens.", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
		App::Print(APP_VIRTUAL_WIDTH / 2.0f - 230, APP_VIRTUAL_HEIGHT / 2.0f + 90, 
			"The recoil from your ship's turret is the only way to move.", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
		App::Print(APP_VIRTUAL_WIDTH / 2.0f - 200, APP_VIRTUAL_HEIGHT / 2.0f + 60,
			"You have one shot every second. Make it count.", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
		App::Print(APP_VIRTUAL_WIDTH / 2.0f - 200, APP_VIRTUAL_HEIGHT / 2.0f + 30,
			"Stay within the red box. Or be lost to space...", 1, 0, 0, GLUT_BITMAP_HELVETICA_18);
	}

	if (currentState != State::PREGAME) {
		std::string scoreString = "Score: " + std::to_string(score);
		App::Print(20.0f, APP_VIRTUAL_HEIGHT - 50.0f, scoreString.c_str(), 1, 1, 1, GLUT_BITMAP_9_BY_15);
	}

	player->Render(*context);
	deathbox->Render(*context);
	bulletPool->Render(*context);
	enemyPool->Render(*context);
	emitter->Render(*context);

	// Output rendered frame
	context->Flush();
}