#pragma once

// Just a simple interface for use such that we can store generic inherited
// classes in a vector in the GameObject class.
namespace Component {
	struct Component {
		Component() = default;
		virtual ~Component() { };
	};
}