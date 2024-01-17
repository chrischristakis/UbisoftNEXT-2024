#include "stdafx.h"

#include "ParticleEmitter.h"
#include "../Graphics/Mesh.h"
#include "app/app.h"

// -------- PARTICLE -------- //

Particle::Particle(Vector3f position,
				   Vector3f velocity,
				   float ttl,
				   float scale,
				   Vector3f color) 
				:
				   position(position), 
				   color(color), 
				   active(false),	
				   velocity(velocity), 
				   ttl(ttl), 
				   scale(scale) { }

bool Particle::Update(float deltaTime) {
	position = position + velocity;

	ttl -= deltaTime;
	if (ttl <= 0)
		active = false;

	return active;
}

void Particle::Render(Graphics& context, const Mesh& mesh) {
	Mat4x4 model = Transform::Translate(Mat4x4::Identity(), position);
	model = Transform::Scale(model, { scale, scale, scale });
	context.RenderMesh(mesh, model, color);
}

// -------- PARTICLE EMITTER -------- //

ParticleEmitter::ParticleEmitter(int MAX_PARTICLES) {
	for (int i = 0; i < MAX_PARTICLES; i++) {
		_particlePool.push_back(Particle({ 0, 0, 0 }));
		_availableIndices.push(i);
	}
}

void ParticleEmitter::Create(Vector3f position,
							 Vector3f velocity,
							 float ttl,
							 float scale,
							 Vector3f color) {
	Particle* particle = _GetAvailableParticle();

	if (!particle)
		return; // If we can't keep up, just don't create anything.

	particle->position = position;
	particle->velocity = velocity;
	particle->color = color;
	
	particle->ttl = ttl;
	particle->scale = scale;

	particle->active = true;
}

// Use a queue to quickly access and check which particle we can use.
Particle* ParticleEmitter::_GetAvailableParticle() {
	if (_availableIndices.size() < 1)
		return nullptr;

	int index = _availableIndices.front();
	_availableIndices.pop();
	return &_particlePool[index];
}

int ParticleEmitter::GetMaxParticles() {
	return _particlePool.size();
}

void ParticleEmitter::Update(float deltaTime) {
	for (int i = 0; i < _particlePool.size(); i++)
		if (_particlePool[i].active)
			if (!_particlePool[i].Update(deltaTime))
				_availableIndices.push(i);
}

void ParticleEmitter::Render(Graphics& context, const Mesh& mesh) {
	int rendered = 0;
	for (Particle& particle : _particlePool)
		if (particle.active) {
			particle.Render(context, mesh);
			rendered++;
		}
}