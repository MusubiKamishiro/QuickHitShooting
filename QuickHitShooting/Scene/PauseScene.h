#pragma once
#include <memory>
#include "Scene.h"

class Menu;

class PauseScene : public Scene
{
private:
	void (PauseScene::*_updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	std::shared_ptr<Menu> _menu;

public:
	PauseScene();
	~PauseScene();

	void Update(const Peripheral& p);
	void Draw();
};

