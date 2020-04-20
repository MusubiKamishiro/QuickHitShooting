#pragma once
#include <memory>
#include <array>
#include "Scene.h"
#include "../Geometry.h"


class GamePlayingScene : public Scene
{
private:
	void (GamePlayingScene::*_updater)(const Peripheral& p);
	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);
	
	void (GamePlayingScene::*_drawer)();
	void RoundDraw();
	void ResultDraw();

public:
	GamePlayingScene();
	~GamePlayingScene();

	void Update(const Peripheral& p);
	void Draw();
};
