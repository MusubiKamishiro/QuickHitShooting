#pragma once
#include "Scene.h"
#include <memory>
#include <array>
#include "../Geometry.h"
#include "../Gun.h"

class TrimString;

//リザルト情報
struct ResultData
{
	int score;					// 獲得スコア
	std::array<int, 3> ranking;	// ランキング
	int hitRate;				// 命中率
	GunStatus gunStatus;		// 銃種
};


class ResultScene : public Scene
{
private:
	void (ResultScene::*_updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	ResultData _resultData;


	std::unique_ptr<TrimString> _trimString;

public:
	ResultScene(const int& score);
	~ResultScene();

	void Update(const Peripheral& p);
	void Draw();
};

