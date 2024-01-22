#pragma once 

#include "stdafx.h"
#include "GameObjects/Enemy.h"

class EnemyPool {
public:
	const int MAX_ENEMIES;
	std::vector<Enemy> enemies;
	unsigned int activeEnemies = 0;

	int GetAvailableEnemy();
	void DestroyEnemy(int index);

	EnemyPool(int MAX_ENEMIES);

	void Update(float deltaTime);
	void Render(Graphics& context);

	void CreateEnemy(Enemy enemy);

	void Clear();
};