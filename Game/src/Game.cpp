#include "stdafx.h"

#include <windows.h> 
#include <math.h>  

#include "app\app.h"

#include "Renderer.h"
#include "Graphics/Graphics.h"
#include "Graphics/Mesh.h"
#include "Graphics/Primitive.h"
#include "Math/Math.h"
#include "Graphics/Camera.h"

Renderer r;
Mesh mesh;
Graphics context;

void Init()
{
	Vector3f t1(5, -5, 0);
	Vector3f t2(-5, -5, 0);
	Vector3f t3(0, 5, 0);

	Primitive t;
	t.PushVertex(t1);
	t.PushVertex(t2);
	t.PushVertex(t3);

	mesh.push_back(t);
}

void Update(float deltaTime)
{
	context.Update();
}

float i = 0;
void Render()
{	
	i += 0.5f;

	Mat4x4 model;
	//model = Transform::RotateY(Mat4x4::Identity(), i);
	context.RenderMesh(mesh, model);
	
	context.Flush();
}

void Shutdown()
{
}