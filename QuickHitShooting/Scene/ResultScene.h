#pragma once
#include "Scene.h"
#include <memory>
#include <vector>
#include <array>
#include <string>
#include "../Geometry.h"
#include "../Gun.h"

class TrimString;
class Menu;
class Keyboard;

// リザルト情報
struct ResultData
{
	std::array<std::pair<std::string, int>, 3> ranking;	// ランキング
	int score;					// 獲得スコア
	float hitRate;				// 命中率
	GunStatus gunStatus;		// 銃種
	std::string name;
};

// 数値のデータ
struct NumData
{
	NumData() : num(0.0f), nowDigit(0), digit(0), digitNums() {};

	float num;					// 数値
	unsigned int nowDigit;		// カウント中の桁数
	unsigned int digit;			// データの桁数
	std::vector<int> digitNums;	// 桁ごとの数値
};

class ResultScene : public Scene
{
private:
	void (ResultScene::*_updater)(const Peripheral& p);
	
	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void ScoreUpdate(const Peripheral& p);
	void HitRateUpdate(const Peripheral& p);
	void RankinUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	bool SaveRanking();

	ResultData _resultData;

	std::unique_ptr<TrimString> _trimString;
	std::shared_ptr<Menu> _menu;
	std::shared_ptr<Keyboard> _keyboard;

	int _resultBg;
	int _resultBd;

	int _time;

	NumData _score;		// 得点
	NumData _hitRate;	// 命中率

	///桁数を確認し、NamDataに値を反映させる
	///@param numDara	数値データ
	///@param num		桁数を確認したい数値
	///@param maxDigit	最大桁数
	void CheckDigit(NumData& numData, const int& num, const int& maxDigit);

	///表示の際に数値をランダムにする際に使用
	///@param maxDigit	最大桁数
	///@param numDara	数値データ
	float RandomCountUp(const unsigned int& maxDigit, const NumData& numData);

public:
	ResultScene(const ResultData& resultData);
	~ResultScene();

	void Update(const Peripheral& p);
	void Draw();
};

