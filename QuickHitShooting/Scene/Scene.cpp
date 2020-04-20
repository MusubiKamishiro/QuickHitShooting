#include "Scene.h"
#include "../Game.h"


Scene::Scene()
{
	_pal = 255;
	_scrSize = Game::Instance().GetScreenSize();
	_sceneTime = 0;
}

Scene::~Scene()
{
}

void Scene::AddSceneTime()
{
	++_sceneTime;
}
