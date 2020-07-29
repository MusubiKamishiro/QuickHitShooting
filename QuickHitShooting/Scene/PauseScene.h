#pragma once
#include <memory>
#include "Scene.h"
#include "../Gun.h"

class Menu;

class PauseScene : public Scene
{
private:
	void (PauseScene::*_updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void RetryUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	std::shared_ptr<Menu> _menu;

	std::shared_ptr<Gun> _gun;
	std::string _stagePath;

public:
	PauseScene(const std::shared_ptr<Gun>& gun, const std::string& stagePath);
	~PauseScene();

	void Update(const Peripheral& p);
	void Draw();
};

