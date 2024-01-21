#include "stdafx.h"

#include "ParticleEmitter.h"
#include "Mesh.h"
#include "app/app.h"

// -------- PARTICLE -------- //

Particle::Particle(Vector3f position,
				   const Mesh* mesh,
				   Vector3f velocity,
				   float ttl,
				   float scale,
				   Vector3f color) 
				:
				   position(position), 
				   color(color), 	
				   velocity(velocity), 
				   ttl(ttl), 
				   scale(scale),
				   initial_ttl(ttl),
				   mesh(mesh) { }

bool Particle::Update(float deltaTime) {
	position = position + velocity * deltaTime;

	ttl -= deltaTime;
	
	return ttl > 0.0f;
}

void Particle::Render(Graphics& context) {
	Mat4x4 model = Math::Transform::Translate(Mat4x4::Identity(), position);
	model = Math::Transform::Scale(model, { scale, scale, scale });
	context.RenderMesh(*mesh, model, color * (ttl / initial_ttl));
}


// -------- PARTICLE EMITTER -------- //

ParticleEmitter::ParticleEmitter(int MAX_PARTICLES) {
	for (int i = 0; i < MAX_PARTICLES; i++)
		_particlePool.push_back(Particle({ 0, 0, 0 }, &Meshes::CUBE));
	_numActive = 0;
}

void ParticleEmitter::Create(Particle p) {
	int index = _GetAvailableParticle();

	if (index == -1)
		return; // If we can't keep up, just don't create anything.

	_particlePool[index] = p;
}

// We keep particles that are active at the front of the pool,
// so we can simply index.
int ParticleEmitter::_GetAvailableParticle() {
	
	// No inactive particles available
	if (_numActive == _particlePool.size())
		return -1;

	return _numActive++;
}

// Particles in the pool that are inactive are contiguous starting from index = _numActive
// This reduce cache misses by keeping all active particles together.
void ParticleEmitter::_DestroyParticle(int index) {
	assert(_numActive > 0);
	
	Particle lastActive = _particlePool[_numActive - 1];
	_particlePool[_numActive - 1] = _particlePool[index];  // Moved to inactive half of vector
	_particlePool[index] = lastActive;
	_numActive--;
}

int ParticleEmitter::GetMaxParticles() {
	return _particlePool.size();
}

void ParticleEmitter::Update(float deltaTime) {
	for (int i = 0; i < _numActive; i++) {
		if (!_particlePool[i].Update(deltaTime))
			_DestroyParticle(i);
	}
}

void ParticleEmitter::Render(Graphics& context) {
	for (int i = 0; i < _numActive; i++)
		_particlePool[i].Render(context);
}

void ParticleEmitter::Clear() {
	_numActive = 0;
}