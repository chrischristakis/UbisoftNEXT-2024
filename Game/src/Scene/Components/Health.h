#pragma once

#include "Component.h"

namespace Component {
	class Health : public Component {
	public:
		int health;
		bool invincible;

		Health(int health);

		bool IsDead();
		void Hit(int damage);
	};
}