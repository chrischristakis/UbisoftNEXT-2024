#include "stdafx.h"

#include "Cooldown.h"

namespace Component {
	CooldownInfo::CooldownInfo() : CooldownInfo(0, 0) { }
	CooldownInfo::CooldownInfo(float cooldown, float timer) : cooldown(cooldown), timer(timer) { }
	
	bool CooldownInfo::Ready() {
		return timer <= 0.0f;
	}

	void CooldownInfo::Refresh() {
		timer = cooldown;
	}

	void CooldownInfo::Reset() {
		timer = 0.0f;
	}

	void CooldownInfo::Update(float deltaTime) {
		if (timer > 0.0f)
			timer -= deltaTime;
	}

	void Cooldown::Update(float deltaTime) {
		for (auto& pair : cooldowns)
			pair.second.Update(deltaTime);
	}

	CooldownInfo& Cooldown::GetCooldown(const std::string& s) {
		return cooldowns.at(s);
	}

	CooldownInfo& Cooldown::CreateCooldown(const std::string& s, float cooldown, float timer) {
		assert(cooldowns.find(s) == cooldowns.end() && "Cooldown already exists");
		cooldowns[s] = CooldownInfo(cooldown, timer);
		return cooldowns.at(s);
	}

	void Cooldown::ResetAll() {
		for (auto& pair : cooldowns)
			pair.second.Reset();
	}
}