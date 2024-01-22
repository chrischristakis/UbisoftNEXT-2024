#pragma once

#include "GameObject.h"
#include "../Components/Transform.h"
#include "../../Math/Math.h"
#include "../../Graphics/ParticleEmitter.h"
#include "../../Graphics/Camera.h"
#include "../BulletPool.h"

class Player : public GameObject {
private:
	Camera* _camera;
	ParticleEmitter* _emitter;
	BulletPool* _bulletPool;

	// Initial conditions
	Vector2f _initialPos;
public:
	Vector3f cursorPos;

	Player(Vector2f position, Camera* scene, ParticleEmitter* emitter, BulletPool* bulletpool);

	void Update(float deltaTime) override;
	void Render(Graphics& context) override;
	void RenderAimLine(Graphics& context); // renders out the line that indicates where they player is aiming
	void RenderLives(Graphics& context); // renders out the line that indicates where they player is aiming

	void UpdateCursorPos();
	void UpdateCooldowns(float deltaTime);
	void ProcessInput();

	void FireBullet();

	void TriggerHit();

	void Reset();

};