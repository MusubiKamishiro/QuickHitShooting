#include <DxLib.h>
#include <cmath>
#include <random>
#include "ResultScene.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "../Peripheral.h"

#include "../Game.h"
#include "../Loader/FileSystem.h"
#include "../Loader/ImageLoader.h"
#include "../Loader/SoundLoader.h"
#include "../TrimString.h"


constexpr int maxDigit = 6;	// ÉXÉRÉAÇÃç≈ëÂåÖêî

ResultScene::ResultScene(const ResultData& resultData)
{
	_pal = 0;
	_time = 0;
	_nowDigit = 0;
	_scoreDigit = 0;
	_trimString = std::make_unique<TrimString>();
	_num.clear();

	_resultData = resultData;
	_resultScore = 0;

	CheckScore(_resultData.score);

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
		_pal +=20;
	}
}

void ResultScene::FadeoutUpdate(const Peripheral & p)
{
	if (_pal <= 0)
	{
		SceneManager::Instance().ChangeScene(std::make_unique<TitleScene>());
	}
	else
	{
		_pal -= 20;
	}
}

void ResultScene::ScoreUpdate(const Peripheral& p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT) || (_nowDigit >= _scoreDigit) || (_nowDigit == maxDigit))
	{
		_resultScore = _resultData.score;

		_updater = &ResultScene::WaitUpdate;
	}
	else
	{
		_resultScore = 0;
		for (unsigned int i = 0; i < maxDigit; ++i)
		{
			_resultScore *= 10;

			if ((maxDigit - _nowDigit) <= i)
			{
				_resultScore += _num[maxDigit - (i + 1)];
			}
			else
			{
				std::random_device seed;
				std::mt19937 engine(seed());
				_resultScore += engine() % 10;
			}
		}
	}

	if (++_time % 60 == 0)
	{
		++_nowDigit;
	}
}

void ResultScene::WaitUpdate(const Peripheral & p)
{
	
}

void ResultScene::CheckScore(const int& inscore)
{
	int score = inscore;
	while (score != 0)
	{
		int sscore = score / 10;
		_num.push_back(score - sscore * 10);
		score = sscore;
		++_scoreDigit;
	}

	while(_num.size() < maxDigit)
	{
		_num.push_back(0);
	}
}

void ResultScene::Update(const Peripheral& p)
{


	(this->*_updater)(p);
}

void ResultScene::Draw()
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0xffffff, true);
	
	_trimString->ChangeFontSize(50);
	std::string s = "score %" + std::to_string(maxDigit) + "d";
	DxLib::DrawFormatString(0, 300, 0x000000, s.c_str(), _resultScore);
	DxLib::DrawFormatString(0, 400, 0x000000, "ñΩíÜó¶ %.2f", _resultData.hitRate);
	DxLib::DrawFormatString(700, 300, 0x000000, "1à  %s %d", _resultData.ranking[0].first.c_str(), _resultData.ranking[0].second);
	DxLib::DrawFormatString(700, 400, 0x000000, "2à  %s %d", _resultData.ranking[1].first.c_str(), _resultData.ranking[1].second);
	DxLib::DrawFormatString(700, 500, 0x000000, "3à  %s %d", _resultData.ranking[2].first.c_str(), _resultData.ranking[2].second);

	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);
}
