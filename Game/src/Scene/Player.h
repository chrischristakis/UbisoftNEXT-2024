#pragma once

#include "../Math/Math.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Graphics.h"

class Player {
private:
	Camera* _camera;
public:
	Vector3f position;

	Player(Camera* camera);

	void Render(Graphics& context);
	void Update(float deltaTime);
	void ProcessInput();
};