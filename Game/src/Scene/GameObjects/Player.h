#pragma once

#include "GameObject.h"
#include "../Components/Transform.h"
#include "../../Graphics/ParticleEmitter.h"
#include "../../Math/Math.h"

class Player : public GameObject {
private:
	Camera* _camera;
	ParticleEmitter* _emitter;

	void _ProcessInput(Component::Transform& transform);
public:
	Player(Vector3f position, Camera* camera, ParticleEmitter* emitter);

	void Update(float deltaTime) override;
	void Render(Graphics& context) override;
};