#include <DxLib.h>
#include <cmath>
#include <random>
#include "ResultScene.h"
#include "SceneManager.h"
#include "SelectScene.h"
#include "../Peripheral.h"

#include "../Game.h"
#include "../Loader/FileSystem.h"
#include "../Loader/ImageLoader.h"
#include "../Loader/SoundLoader.h"
#include "../TrimString.h"
#include "../Menu.h"

#include "../Keyboard.h"


constexpr int _maxScoreDigit = 6;	// スコアの最大桁数
constexpr int _maxHitRateDigit = 5;	// 命中率の最大桁数

ResultScene::ResultScene(const ResultData& resultData)
{
	_pal = 0;
	_time = 0;
	_trimString = std::make_unique<TrimString>();
	_keyboard.reset(new Keyboard());
	_menu.reset(new Menu());

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/result.png", data);
	_resultBg = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/plate/resultBd.png", data);
	_resultBd = data.GetHandle();

	Game::Instance().GetFileSystem()->Load("img/sample02.png", data);
	int img = data.GetHandle();
	_menu->AddMenuList("ReTry", Vector2<int>(_scrSize.x/2 - 330, _scrSize.y - 150), Vector2<int>(_scrSize.x/2 - 30, _scrSize.y), img);
	Game::Instance().GetFileSystem()->Load("img/sample03.png", data);
	img = data.GetHandle();
	_menu->AddMenuList("BackSelect", Vector2<int>(_scrSize.x/2 + 30, _scrSize.y - 150), Vector2<int>(_scrSize.x/2 + 330, _scrSize.y), img);

	_resultData = resultData;

	CheckDigit(_score, _resultData.score, _maxScoreDigit);
	CheckDigit(_hitRate, _resultData.hitRate * 100, _maxHitRateDigit);

	_updater = &ResultScene::FadeinUpdate;
}

ResultScene::~ResultScene()
{
}

void ResultScene::FadeinUpdate(const Peripheral & p)
{
	if (_pal >= 255)
	{
		_pal = 255;
		_updater = &ResultScene::ScoreUpdate;
	}
	else
	{
		_pal += 20;
	}
}

void ResultScene::FadeoutUpdate(const Peripheral & p)
{
	if (_pal <= 0)
	{
		SceneManager::Instance().ChangeScene(std::make_unique<SelectScene>());
	}
	else
	{
		_pal -= 20;
	}
}

void ResultScene::ScoreUpdate(const Peripheral& p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT) || (_score.nowDigit >= _score.digit) || (_score.nowDigit == _maxScoreDigit))
	{
		_score.num = _resultData.score;

		_updater = &ResultScene::HitRateUpdate;
	}
	else
	{
		_score.num = RandomCountUp(_maxScoreDigit, _score);
	}

	if (++_time % 60 == 0)
	{
		++_score.nowDigit;
	}
}

void ResultScene::HitRateUpdate(const Peripheral& p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT) || (_hitRate.nowDigit >= _hitRate.digit) || (_hitRate.nowDigit == _maxHitRateDigit))
	{
		_hitRate.num = _resultData.hitRate;

		_updater = &ResultScene::WaitUpdate;
		if (_score.num > _resultData.ranking[2].second)
		{
			_updater = &ResultScene::RankinUpdate;
		}
	}
	else
	{
		_hitRate.num = (RandomCountUp(_maxHitRateDigit - 1, _hitRate) / 100);
	}

	if (++_time % 60 == 0)
	{
		++_hitRate.nowDigit;
	}
}

void ResultScene::RankinUpdate(const Peripheral& p)
{
	_menu->Update(p);

	if (_keyboard->Update(p))
	{
		for (int i = (_resultData.ranking.size() - 1); i >= 0; --i)
		{
			if (_resultData.ranking[i].second < _score.num)
			{
				if (i != (_resultData.ranking.size() - 1))
				{
					_resultData.ranking[i + 1].first  = _resultData.ranking[i].first;
					_resultData.ranking[i + 1].second = _resultData.ranking[i].second;
				}

				_resultData.ranking[i].first  = _keyboard->GetName();
				_resultData.ranking[i].second = _resultData.score;
			}
		}
		/// ランキングデータをセーブする
		SaveRanking();
		_updater = &ResultScene::WaitUpdate;
	}
}

void ResultScene::WaitUpdate(const Peripheral & p)
{
	_menu->Update(p);

	if (_menu->CheckClick("ReTry", p))
	{

	}
	else if (_menu->CheckClick("BackSelect", p))
	{
		_updater = &ResultScene::FadeoutUpdate;
	}
}

bool ResultScene::SaveRanking()
{
	StageData data;
	Game::Instance().GetFileSystem()->Load(_resultData.name, data);

	FILE* file;
	/// フォルダーで指定したファイルを開く
	if (fopen_s(&file, _resultData.name.c_str(), "wb") == 0)
	{
		/// ランキングデータの書き込み
		char name[3];
		for (int i = 0; i < _resultData.ranking.size(); ++i)
		{
			/// スコアの書き込み
			fwrite(&_resultData.ranking[i].second, sizeof(int), 1, file);
			/// 文字の取得を行っている
			for (int c = 0; c < sizeof(name) / sizeof(name[0]); ++c)
			{
				name[c] = _resultData.ranking[i].first[c];
			}
			/// プレイヤー名の書き込み
			fwrite(name, (sizeof(char) * 3), 1, file);
		}
		/// 書き込むウェーブ数の設定
		int targetCnt = 0;
		int waveCnt	  = data.GetStageData().targetData.size();
		fwrite(&waveCnt, sizeof(int), 1, file);

		for (int w = 0; w < waveCnt; ++w)
		{
			/// 書き込むターゲット数の設定
			targetCnt = data.GetStageData().targetData[w].size();
			fwrite(&targetCnt, sizeof(int), 1, file);
			for (int t = 0; t < targetCnt; ++t)
			{
				/// ターゲットデータの書き込み
				fwrite(&data.GetStageData().targetData[w][t].type,		 sizeof(unsigned char), 1, file);
				fwrite(&data.GetStageData().targetData[w][t].dispTime,	 sizeof(unsigned int), 1, file);
				fwrite(&data.GetStageData().targetData[w][t].banishTime, sizeof(unsigned int), 1, file);
				fwrite(&data.GetStageData().targetData[w][t].pos.x,		 sizeof(int), 1, file);
				fwrite(&data.GetStageData().targetData[w][t].pos.y,		 sizeof(int), 1, file);
			}
		}
		fclose(file);
		return true;
	}
	return false;
}

void ResultScene::CheckDigit(NumData& numData, const int& num, const int& maxDigit)
{
	int score = num;
	while (score != 0)
	{
		int sscore = score / 10;
		numData.digitNums.push_back(score - sscore * 10);
		score = sscore;
		++numData.digit;
	}

	while(_score.digitNums.size() < maxDigit)
	{
		_score.digitNums.push_back(0);
	}
}

float ResultScene::RandomCountUp(const unsigned int& maxDigit, const NumData& numData)
{
	float num = 0.f;

	for (unsigned int i = 0; i < maxDigit; ++i)
	{
		num *= 10;

		if ((maxDigit - numData.nowDigit) <= i)
		{
			num += numData.digitNums[maxDigit - (i + 1)];
		}
		else
		{
			std::random_device seed;
			std::mt19937 engine(seed());
			num += engine() % 10;
		}
	}

	return num;
}

void ResultScene::Update(const Peripheral& p)
{
	(this->*_updater)(p);
}

void ResultScene::Draw()
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0xffffff, true);
	
	DxLib::DrawGraph(0, 0, _resultBg, true);

	DxLib::DrawGraph(40, _scrSize.y / 10, _resultBd, true);

	_trimString->ChangeFontSize(50);
	std::string s = "score %" + std::to_string(_maxScoreDigit) + "d";
	DxLib::DrawFormatString(100, 150, 0x000000, s.c_str(), static_cast<int>(_score.num));
	s = "HitRate %" + std::to_string(_maxHitRateDigit) + ".2f";
	DxLib::DrawFormatString(100, 250, 0x000000, s.c_str(), _hitRate.num);
	DxLib::DrawFormatString(900, 150, 0x000000, "1位 %s %d", _resultData.ranking[0].first.c_str(), _resultData.ranking[0].second);
	DxLib::DrawFormatString(900, 250, 0x000000, "2位 %s %d", _resultData.ranking[1].first.c_str(), _resultData.ranking[1].second);
	DxLib::DrawFormatString(900, 350, 0x000000, "3位 %s %d", _resultData.ranking[2].first.c_str(), _resultData.ranking[2].second);

	if (_updater == &ResultScene::RankinUpdate)
	{
		_keyboard->Draw();
	}
	else if (_updater == &ResultScene::WaitUpdate)
	{
		_menu->Draw();
	}

	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);
}
