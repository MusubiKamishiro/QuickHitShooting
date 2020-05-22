#pragma once
#include <memory>
#include <array>
#include <vector>
#include "Scene.h"
#include "../Geometry.h"

#include "../Loader/StageLoader.h"

class Gun;
class Enemy;
class CollisionDetector;
class Menu;
class TrimString;

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

	bool CreateEnemy();
	std::shared_ptr<Enemy> GetEnemyInfo(const TargetData& target);
	

	std::shared_ptr<Gun> _gun;
	std::shared_ptr<CollisionDetector> _cd;
	std::shared_ptr<Menu> _menu;
	std::shared_ptr<TrimString> _trimString;

	/// 仮で複数の敵を出現させている
	std::vector<std::shared_ptr<Enemy>> _enemies;

	StageData _stageData;

	int _waveCnt;	// ウェーブカウント
	int _score;		// 獲得スコア

	float _hitCount;
	float _shotCount;

	bool _hitFlag;

public:
	GamePlayingScene(const GunStatus& gunState, const StageData& stageData);
	~GamePlayingScene();

	void Update(const Peripheral& p);
	void Draw();
};
