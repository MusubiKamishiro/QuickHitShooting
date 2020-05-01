#pragma once
#include <memory>
#include <array>
#include "Scene.h"
#include "../Geometry.h"

class Gun;

class GamePlayingScene : public Scene
{
private:
	void (GamePlayingScene::*_updater)(const Peripheral& p);
	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);
	
	void (GamePlayingScene::*_drawer)();
	void TestDraw();

	std::shared_ptr<Gun> _gun;

public:
	GamePlayingScene();
	~GamePlayingScene();

	void Update(const Peripheral& p);
	void Draw();
};
