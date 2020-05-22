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


ResultScene::ResultScene(const int& score)
{
	_pal = 0;
	_trimString = std::make_unique<TrimString>();

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
	
	_trimString->ChangeFontSize(100);

	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);
}
