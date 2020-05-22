#include <DxLib.h>
#include "ResultScene.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "../Peripheral.h"

#include "../Game.h"
#include "../Loader/FileSystem.h"
#include "../Loader/ImageLoader.h"
#include "../Loader/SoundLoader.h"
#include "../TrimString.h"


ResultScene::ResultScene(const ResultData& resultData)
{
	_pal = 0;
	_trimString = std::make_unique<TrimString>();

	_resultData = resultData;

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
		_updater = &ResultScene::WaitUpdate;
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

void ResultScene::WaitUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		_updater = &ResultScene::FadeoutUpdate;
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
	DxLib::DrawFormatString(0, 100, 0x000000, "score %d", _resultData.score);
	DxLib::DrawFormatString(0, 200, 0x000000, "–½’†—¦ %.2f", _resultData.hitRate);
	DxLib::DrawFormatString(0, 300, 0x000000, "1ˆÊ %s %d", _resultData.ranking[0].first.c_str(), _resultData.ranking[0].second);
	DxLib::DrawFormatString(0, 400, 0x000000, "2ˆÊ %s %d", _resultData.ranking[1].first.c_str(), _resultData.ranking[1].second);
	DxLib::DrawFormatString(0, 500, 0x000000, "3ˆÊ %s %d", _resultData.ranking[2].first.c_str(), _resultData.ranking[2].second);

	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);
}
