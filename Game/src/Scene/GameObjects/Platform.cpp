#include "stdafx.h"

#include "Platform.h"
#include "../Components/Transform.h"
#include "../Components/Box2D.h"

Platform::Platform(Vector3f position, Vector3f scale) {
	CreateComponent<Component::Transform>(Component::Transform{position, scale});
	CreateComponent<Component::Box2D>(Component::Box2D{Vector2f(position), Vector2f(scale)});
};

void Platform::Render(Graphics& context) {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	context.RenderMesh(Meshes::CUBE, transform.CreateModelMatrix());
}