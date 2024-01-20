#pragma once

#include "GameObject.h"
#include "../Components/Transform.h"
#include "../../Graphics/ParticleEmitter.h"
#include "../../Math/Math.h"

class Player : public GameObject {
private:
	Camera* _camera;

public:
	Vector3f cursorPos;

	Player(Vector3f position, Camera* camera);

	void Update(float deltaTime) override;
	void LateUpdate(float deltaTime) override;
	void Render(Graphics& context) override;
	void RenderAimLine(Graphics& context); // renders out the line that indicates where they player is aiming

	void UpdateCursorPos();
	void ProcessInput();

};