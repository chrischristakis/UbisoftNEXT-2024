#pragma once

#include "stdafx.h"
#include <typeinfo>
#include "../../Graphics/Graphics.h"
#include "../Components/Component.h"

class GameObject {
private:
	std::unordered_map<std::string, std::unique_ptr<Component::Component>> _components;
public:
	virtual void Update(float deltaTime) { }
	virtual void Render(Graphics& context) { }

	template <class T>
	void CreateComponent(T component) {
		std::string componentId = typeid(T).name();
		assert(_components.count(componentId) == 0 && "Component already exists");
		_components.insert({ componentId, std::make_unique<T>(component)});
	}

	template <class T>
	void DeleteComponent() {
		std::string componentId = typeid(T).name();
		assert(_components.count(componentId) != 0 && "Component doesn't exist...");
		_components.erase(componentId);
	}

	template <class T>
	T* GetComponent() {
		std::string componentId = typeid(T).name();
		assert(_components.count(componentId) != 0 && "Component doesn't exist...");
		return static_cast<T*>(_components[componentId].get());
	}
};