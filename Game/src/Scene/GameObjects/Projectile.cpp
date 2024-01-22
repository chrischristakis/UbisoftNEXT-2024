#include "stdafx.h"
#include "App/App.h"

#include "Projectile.h"
#include "../Components/Transform.h"
#include "../Components/CircleCollider.h"
#include "../Components/Physics.h"

static const float HOSTILE_PROJECTILE_RADIUS = 0.1f;
static const float PLAYER_PROJECTILE_RADIUS = 0.3f;
static const Vector3f PLAYER_PROJECTILE_SCALE{ 0.2f, 0.8f, 0.2f };
static const Vector3f HOSTILE_PROJECTILE_SCALE{ 0.2f, 0.2f, 0.2f };

Projectile::Projectile(): Projectile({0, 0}, {0, 0}) {}
Projectile::Projectile(Vector2f position, Vector2f velocity, float ttl, bool hostile): ttl(ttl), hostile(hostile) {
	Component::Transform& transform = *CreateComponent<Component::Transform>(Component::Transform{ Vector3f(position, 0.0f), Vector3f(1, 1, 1)});
	CreateComponent<Component::Physics>(Component::Physics{ velocity });
	Component::CircleCollider& collider = *CreateComponent<Component::CircleCollider>(Component::CircleCollider(position, 1.0f));

	if (velocity.x != 0 && velocity.y != 0) {
		float num = Math::Dot(velocity, Vector2f(0, 1));
		float dem = Math::Magnitude(velocity);
		float angle = Math::RadToDeg(acos(num / dem));
		if (velocity.x > 0)
			angle = -angle;
		transform.rotation.z = angle;
	}

	if (!hostile) {
		transform.scale = PLAYER_PROJECTILE_SCALE;
		collider.radius = PLAYER_PROJECTILE_RADIUS;
	}
	else {
		transform.scale = HOSTILE_PROJECTILE_SCALE;
		collider.radius = HOSTILE_PROJECTILE_RADIUS;
	}
}

void Projectile::Update(float deltaTime) {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	Component::CircleCollider& collider = *GetComponent<Component::CircleCollider>();
	Component::Physics& physics = *GetComponent<Component::Physics>();

	// make bullets spin
	transform.rotation.y = transform.rotation.y + 5;
	if (transform.rotation.y > 360.0f)
		transform.rotation.y = 0;

	transform.position = transform.position + Vector3f(physics.velocity, 0.0f) * deltaTime;
	collider.position = Vector2f(transform.position);

	if (ttl > 0.0f)
		ttl -= deltaTime;
}

void Projectile::Render(Graphics& context) {
	Component::Transform& transform = *GetComponent<Component::Transform>();
	Component::CircleCollider& collider = *GetComponent<Component::CircleCollider>();
	if(hostile)
		context.RenderMesh(Meshes::CUBE, transform.CreateModelMatrix(), {1, 0, 0});
	else
		context.RenderMesh(Meshes::CONE, transform.CreateModelMatrix(), { 0, 1, 1 });
	collider.Render(context);

}