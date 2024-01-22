#include "stdafx.h"

#include "Health.h"

namespace Component {
	Health::Health(int health) : health(health), invincible(false) { }

	bool Health::IsDead() {
		return health <= 0;
	}

	void Health::Hit(int damage) {
		if (!invincible)
			health -= damage;
	}
}