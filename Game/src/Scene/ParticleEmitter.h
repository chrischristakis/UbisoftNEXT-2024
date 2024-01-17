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
	bool active;

	Particle(Vector3f position,
			 Vector3f velocity = {0, 0, 0},
			 float ttl = 10000.0f,
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

	// A vector that stores what indices in the particle
	// pool are inactive and available for use
	std::queue<int> _availableIndices;

	// Find an inactive particle in the pool for use
	Particle* _GetAvailableParticle();

public:

	ParticleEmitter(int MAX_PARTICLES);

	// Prepare a particle for use
	void Create(Vector3f position,
				Vector3f velocity,
				float ttl = 1000.0f,
				float scale = 1.0f,
				Vector3f color = {1, 1, 1});

	int GetMaxParticles();

	void Update(float deltaTime);
	void Render(Graphics& context, const Mesh&);
};