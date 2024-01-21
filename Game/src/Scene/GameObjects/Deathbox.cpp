#include "stdafx.h"

#include "Deathbox.h"
#include "../../Math/Math.h"

Deathbox::Deathbox(float x, float y, float w, float h): color(1, 0, 0) {
	CreateComponent<Component::Transform>(Component::Transform{
		Vector3f(x, y, 0.0f), Vector3f(w, h, 1.0f) }
	);
}

bool Deathbox::InDeathbox(Vector2f position) {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	return (
		position.x >= transform.position.x - transform.scale.x / 2 &&
		position.y >= transform.position.y - transform.scale.y / 2 &&
		position.x <= transform.position.x + transform.scale.x / 2 &&
		position.y <= transform.position.y + transform.scale.y / 2
	);
}

void Deathbox::Render(Graphics& context) {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	Mat4x4 model = Math::Transform::Translate(Mat4x4::Identity(), transform.position);
	model = Math::Transform::Scale(model, transform.scale);
	context.RenderUnclippedMesh(Meshes::QUAD, model, color);
	model = Math::Transform::Translate(Mat4x4::Identity(), transform.position);
	model = Math::Transform::Scale(model, transform.scale * 1.02f);
	context.RenderUnclippedMesh(Meshes::QUAD, model, color);
}