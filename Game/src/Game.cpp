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
	scene->Update(deltaTime);
}

void Render()
{	
	scene->Render();
}

void Shutdown()
{
	delete scene;
}