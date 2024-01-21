#include "stdafx.h"

#include "Transform.h"

namespace Component {
	Transform::Transform(Vector3f position, Vector3f scale, Vector3f rotation) :
		position(position), scale(scale), rotation(rotation) { }
	Transform::Transform() : Transform({ 0, 0, 0 }) { }

	Mat4x4 Transform::CreateModelMatrix() {
		Mat4x4 model = Math::Transform::Translate(Mat4x4::Identity(), position);
		model = Math::Transform::RotateZ(model, rotation.z);
		model = Math::Transform::RotateY(model, rotation.y);
		model = Math::Transform::RotateX(model, rotation.x);
		model = Math::Transform::Scale(model, scale);

		return model;
	}
}