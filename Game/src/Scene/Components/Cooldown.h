#pragma once

#include "stdafx.h"
#include "Component.h"

namespace Component {
	struct CooldownInfo {
		float timer;
		float cooldown;
		CooldownInfo();
		CooldownInfo(float cooldown, float timer);
		bool Ready();
		void Reset(); // Sets timer to 0
		void Refresh(); // Sets timer to cooldown
		void Update(float deltaTime);
	};

	class Cooldown: public Component {
	private:
		std::unordered_map<std::string, CooldownInfo> cooldowns;
	public:
		Cooldown() = default;
		void Update(float deltaTime);
		
		CooldownInfo& GetCooldown(const std::string& s);
		CooldownInfo& CreateCooldown(const std::string& s, float COOLDOWN, float timer = 0.0f);
		void ResetAll();
	};
}