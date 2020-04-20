#include "SceneManager.h"
#include "Scene.h"
#include "TitleScene.h"


SceneManager::SceneManager()
{
	scene.emplace_front(std::make_unique<TitleScene>());
}

SceneManager::~SceneManager()
{
}

void SceneManager::ChangeScene(std::unique_ptr<Scene> newScene)
{
	while (scene.size())
	{
		scene.pop_front();
	}
	scene.emplace_front(std::move(newScene));
}

void SceneManager::PushScene(std::unique_ptr<Scene> newScene)
{
	scene.emplace_front(std::move(newScene));
}

void SceneManager::PopScene()
{
	scene.pop_front();
}

void SceneManager::Update(Peripheral & p)
{
	scene.front()->AddSceneTime();
	scene.front()->Update(p);
}

void SceneManager::Draw()
{
	for (int i = (scene.size() - 1); i >= 0; --i)
	{
		scene.at(i)->Draw();
	}
}
