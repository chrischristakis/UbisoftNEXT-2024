#include "stdafx.h"

#include "../../Config.h"
#include "CircleCollider.h"
#include "../../Math/Math.h"

namespace Component {
	CircleCollider::CircleCollider(Vector2f position, float radius):position(position), radius(radius) { }

	bool CircleCollider::CollidesWith(const CircleCollider& other) {
		return (Math::Magnitude(position - other.position) <= radius + other.radius);
	}

	void CircleCollider::Render(Graphics& context) {
#ifdef NEXT_DEBUG
		static const int DETAIL = 20;
		Primitive p;
		float partition = 360.0f / DETAIL;
		for (int i = 0; i < DETAIL; i++) {
			p.PushVertex(Vertex{
				radius * cos(Math::DegToRad(partition * i)),
				radius * sin(Math::DegToRad(partition * i)),
				0.0f
			});
		}
		Mesh m { p };
		Mat4x4 model = Math::Transform::Translate(Mat4x4::Identity(), Vector3f(position, 0.0f));
		context.RenderMesh(m, model);
#endif
	}
}