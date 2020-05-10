#pragma once
#include <memory>
#include <array>
#include "Scene.h"
#include "../Geometry.h"

class Gun;
class Enemy;
class CollisionDetector;

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
	std::shared_ptr<Enemy> _enemy;
	std::shared_ptr<CollisionDetector> _cd;

	bool hitFlag;

public:
	GamePlayingScene();
	~GamePlayingScene();

	void Update(const Peripheral& p);
	void Draw();
};
