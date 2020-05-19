#pragma once
#include <memory>
#include <array>
#include <vector>
#include "Scene.h"
#include "../Geometry.h"

class Gun;
class Enemy;
class CollisionDetector;

struct TargetData;

class GamePlayingScene : public Scene
{
private:
	void (GamePlayingScene::*_updater)(const Peripheral& p);
	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);
	
	void (GamePlayingScene::*_drawer)();
	void TestDraw();

	bool CreateEnemy();
	std::shared_ptr<Enemy> GetEnemyInfo(const TargetData& target);
	

	std::shared_ptr<Gun> _gun;
	std::shared_ptr<CollisionDetector> _cd;

	/// ‰¼‚Å•¡”‚Ì“G‚ğoŒ»‚³‚¹‚Ä‚¢‚é
	std::vector<std::shared_ptr<Enemy>> _enemies;

	int _waveCnt;

	bool hitFlag;

public:
	GamePlayingScene();
	~GamePlayingScene();

	void Update(const Peripheral& p);
	void Draw();
};
