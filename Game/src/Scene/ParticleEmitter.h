#pragma once

#include <queue>
#include "../Math/Math.h"
#include "../Graphics/Graphics.h"

struct Particle {
public:
	Vector3f position;
	Vector3f color;
	Vector3f velocity;
	float ttl; // Time to live in milliseconds
	float scale;

	Particle(Vector3f position,
			 Vector3f velocity = {0, 0, 0},
			 float ttl = 0.0f,
			 float scale = 1.0f,
			 Vector3f color = { 1, 1, 1 });

	bool Update(float deltaTime);  // Returns whether the particle is active.
	void Render(Graphics& context, const Mesh& mesh);
};

// Particle Emitter uses object pooling to reduce fragmentation between particle instances
class ParticleEmitter {
private:

	// Populated with inactive particles at construction
	std::vector<Particle> _particlePool;

	int _numActive;

	// Find an index for an inactive particle in the pool
	int _GetAvailableParticle();
	void _DestroyParticle(int index);

public:

	ParticleEmitter(int MAX_PARTICLES);

	// Prepare a particle for use
	void Create(Vector3f position,
				Vector3f velocity,
				float ttl = 0.0f,
				float scale = 1.0f,
				Vector3f color = {1, 1, 1});

	int GetMaxParticles();

	void Update(float deltaTime);
	void Render(Graphics& context, const Mesh&);
};