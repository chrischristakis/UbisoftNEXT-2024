#pragma once

#include "../Graphics/Graphics.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Primitive.h"
#include "../Graphics/Camera.h"
#include "../Graphics/ParticleEmitter.h"
#include "GameObjects/Player.h"
#include "GameObjects/Deathbox.h"
#include "GameObjects/Enemy.h"
#include "Components/Cooldown.h"
#include "BulletPool.h"
#include "EnemyPool.h"

class Scene {
private:
	Component::Cooldown _cooldowns;
public:
	BulletPool* bulletPool;
	EnemyPool* enemyPool;
	Camera* camera;
	Graphics* context;
	ParticleEmitter* emitter;
	Player* player;
	Deathbox* deathbox;

	int score;

	enum class State {
		PLAYING,
		PREGAME,
		GAME_OVER
	} currentState;

	Scene();
	~Scene();
	void Render();
	void Update(float deltaTime);
	void ProcessInput();
	void DoCollisions(float deltaTime);

	void SpawnEnemy();

	void Restart();
};