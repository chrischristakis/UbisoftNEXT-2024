#include "stdafx.h"

#include <windows.h> 
#include <math.h>  

#include "Scene/Scene.h"

Scene* scene;

void Init()
{
	scene = new Scene();
}

void Update(float deltaTime)
{
	scene->Update();
}

void Render()
{	
	scene->Render();
}

void Shutdown()
{
	delete scene;
}