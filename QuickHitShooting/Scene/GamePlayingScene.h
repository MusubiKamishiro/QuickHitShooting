#pragma once
#include <memory>
#include <array>
#include <vector>
#include "Scene.h"
#include "../Geometry.h"

class Gun;
class Enemy;
class CollisionDetector;
class Menu;

struct TargetData;
struct GunStatus;

class GamePlayingScene : public Scene
{
private:
	void (GamePlayingScene::*_updater)(const Peripheral& p);
	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);
	
	void (GamePlayingScene::*_drawer)();
	void TestDraw();

	void CreateEnemy();
	std::shared_ptr<Enemy> GetEnemyInfo(const TargetData& target);
	

	std::shared_ptr<Gun> _gun;
	std::shared_ptr<CollisionDetector> _cd;
	std::shared_ptr<Menu> _menu;

	/// 仮で複数の敵を出現させている
	std::vector<std::shared_ptr<Enemy>> _enemies;

	int _waveCnt;

	bool hitFlag;

public:
	GamePlayingScene(const GunStatus& gunState);
	~GamePlayingScene();

	void Update(const Peripheral& p);
	void Draw();
};
