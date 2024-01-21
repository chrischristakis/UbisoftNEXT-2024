#include "stdafx.h"
#include "App/App.h"

#include "Projectile.h"
#include "../Components/Transform.h"
#include "../Components/Physics.h"

Projectile::Projectile(): Projectile({0, 0}, {0, 0}) {}
Projectile::Projectile(Vector2f position, Vector2f velocity, float ttl, bool hostile): ttl(ttl), hostile(hostile) {
	Component::Transform& transform = *CreateComponent<Component::Transform>(Component::Transform{ Vector3f(position, 0.0f), Vector3f(1, 1, 1)});
	CreateComponent<Component::Physics>(Component::Physics{ velocity });

	if (velocity.x != 0 && velocity.y != 0) {
		float num = Math::Dot(velocity, Vector2f(0, 1));
		float dem = Math::Distance(velocity);
		float angle = Math::RadToDeg(acos(num / dem));
		if (velocity.x > 0)
			angle = -angle;
		transform.rotation.z = angle;
	}

	if (!hostile)
		transform.scale = Vector3f(0.2f, 0.8f, 0.2f);
	else
		transform.scale = Vector3f(0.2f, 0.2f, 0.2f);
}

void Projectile::Update(float deltaTime) {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	transform.rotation.y = transform.rotation.y + 5;
	if (transform.rotation.y > 360.0f)
		transform.rotation.y = 0;

	Component::Physics& physics = *GetComponent<Component::Physics>();
	transform.position = transform.position + Vector3f(physics.velocity, 0.0f) * deltaTime;

	if (ttl > 0.0f)
		ttl -= deltaTime;
}

void Projectile::Render(Graphics& context) {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	if(hostile)
		context.RenderMesh(Meshes::CUBE, transform.CreateModelMatrix(), {1, 0, 0});
	else
		context.RenderMesh(Meshes::CONE, transform.CreateModelMatrix(), { 0, 1, 1 });
}