#include "stdafx.h"

#include "EnemyPool.h"
#include "Components/Transform.h"

static const float BOUNDS = 15.0f;

EnemyPool::EnemyPool(int MAX_ENEMIES): MAX_ENEMIES(MAX_ENEMIES) {
	for (int i = 0; i < MAX_ENEMIES; i++)
		enemies.push_back(Enemy());
	activeEnemies = 0;
}

int EnemyPool::GetAvailableEnemy() {
	if (activeEnemies >= MAX_ENEMIES)
		return -1;
	return activeEnemies++;
}

void EnemyPool::DestroyEnemy(int index) {
	assert(index >= 0 && index < activeEnemies && "Invalid index to destroy");

	Enemy lastActive = enemies[activeEnemies - 1];
	enemies[activeEnemies - 1] = enemies[index];
	enemies[index] = lastActive;
	activeEnemies--;
}

void EnemyPool::Update(float deltaTime) {
	std::vector<int> toRemove;
	for (int i = 0; i < activeEnemies; i++) {
		Component::Transform& transform = *enemies[i].GetComponent<Component::Transform>();

		if (transform.position.x < BOUNDS && transform.position.x > -BOUNDS &&
			transform.position.y < BOUNDS && transform.position.y > -BOUNDS)
			enemies[i].Update(deltaTime);
		else
			toRemove.push_back(i);
	}

	for (int i : toRemove)
		DestroyEnemy(i);
}

void EnemyPool::Render(Graphics& context) {
	for (int i = 0; i < activeEnemies; i++)
		enemies[i].Render(context);
}

void EnemyPool::CreateEnemy(Enemy enemy) {
	int index = GetAvailableEnemy();
	if (index == -1)
		return;
	enemies[index] = enemy;
}

void EnemyPool::Clear() {
	activeEnemies = 0;
}