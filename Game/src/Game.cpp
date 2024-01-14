#include "stdafx.h"

#include <windows.h> 
#include <math.h>  

#include "app\app.h"

#include "Renderer.h"
#include "Math/Math.h"

Renderer r;
Mat4x4 perspective;
Vector3f pos(0, 0, -30);

void Init()
{
	float aspectRatio = (float)APP_VIRTUAL_WIDTH / APP_VIRTUAL_HEIGHT;
	perspective = Transform::Perspective(45.0f, aspectRatio, 1.0f, 100.0f);
}

void Update(float deltaTime)
{
	if (App::IsKeyPressed('W'))
		pos.z -= 1;
	if (App::IsKeyPressed('S'))
		pos.z += 1;
	if (App::IsKeyPressed('A'))
		pos.x -= 1;
	if (App::IsKeyPressed('D'))
		pos.x += 1;
	if (App::IsKeyPressed('R'))
		pos.y += 1;
	if (App::IsKeyPressed('F'))
		pos.y -= 1;
}

short i = 0;
void Render()
{	
	//std::string str = "Timer: " + std::to_string(i);
	i = (i + 1) % 360;

	// Test 3D
	Vector4f v1(-5, -5, 0, 1);
	Vector4f v2(5, -5, 0, 1);
	Vector4f v3(5, 5, 0, 1);
	Vector4f v4(-5, 5, 0, 1);

	// Model
	Mat4x4 rot;
	rot = Transform::RotateY(rot, i);

	v1 = rot * v1;
	v2 = rot * v2;
	v3 = rot * v3;
	v4 = rot * v4;

	// View
	v1 = v1 + pos;
	v2 = v2 + pos;
	v3 = v3 + pos;
	v4 = v4 + pos;

	// Projection
	v1 = perspective * v1;
	v2 = perspective * v2;
	v3 = perspective * v3;
	v4 = perspective * v4;

	Vector3f v13 = Vector3f(v1.x / v1.w, v1.y / v1.w, v1.z / v1.w);
	Vector3f v23 = Vector3f(v2.x / v2.w, v2.y / v2.w, v2.z / v2.w);
	Vector3f v33 = Vector3f(v3.x / v3.w, v3.y / v3.w, v3.z / v3.w);
	Vector3f v43 = Vector3f(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);

	float scale_x = APP_VIRTUAL_WIDTH / 2.0f;
	float scale_y = APP_VIRTUAL_HEIGHT / 2.0f;

	float screen_x1 = (v13.x + 1) * scale_x;
	float screen_y1 = (v13.y + 1) * scale_y;
	float screen_x2 = (v23.x + 1) * scale_x;
	float screen_y2 = (v23.y + 1) * scale_y;
	float screen_x3 = (v33.x + 1) * scale_x;
	float screen_y3 = (v33.y + 1) * scale_y;
	float screen_x4 = (v43.x + 1) * scale_x;
	float screen_y4 = (v43.y + 1) * scale_y;

	App::DrawLine(screen_x1, screen_y1, screen_x2, screen_y2);
	App::DrawLine(screen_x2, screen_y2, screen_x3, screen_y3);
	App::DrawLine(screen_x3, screen_y3, screen_x4, screen_y4);
	App::DrawLine(screen_x4, screen_y4, screen_x1, screen_y1);
}

void Shutdown()
{
}