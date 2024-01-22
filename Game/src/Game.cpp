#include "stdafx.h"

#include <windows.h> 
#include <math.h>  

#include "Config.h"
#include "Scene/Scene.h"
#include "Tests.h"

Scene* scene;

void Init()
{
#ifdef NEXT_DEBUG
	Tests::RunAllTests();
#endif
	scene = new Scene();
}

void Update(float deltaTime)
{
	// Delta time is coverted to MS
	scene->Update(deltaTime / 1000.0f);
}

void Render()
{	
	scene->Render();
}

void Shutdown()
{
	delete scene;
}