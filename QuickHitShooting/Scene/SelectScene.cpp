#include <DxLib.h>
#include <random>
#include "SelectScene.h"
#include "GamePlayingScene.h"
#include "SceneManager.h"
#include "../Peripheral.h"
#include "../TrimString.h"

#include "../Game.h"
#include "../Loader/FileSystem.h"
#include "../Loader/ImageLoader.h"


void SelectScene::FadeinUpdate(const Peripheral& p)
{
	if (_pal >= 255)
	{
		_pal = 255;
		_updater = &SelectScene::WaitUpdate;
	}
	else
	{
		_pal += 20;
	}
}

void SelectScene::FadeoutUpdate(const Peripheral& p)
{
	if (_pal <= 0)
	{
		
		SceneManager::Instance().ChangeScene(std::make_unique<GamePlayingScene>(_gunState));
	}
	else
	{
		_pal -= 20;
	}
}

void SelectScene::WaitUpdate(const Peripheral& p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		_updater = &SelectScene::FadeoutUpdate;
	}
}

SelectScene::SelectScene()
{
	_pal = 0;
	_trimString = std::make_unique<TrimString>();

	_updater = &SelectScene::FadeinUpdate;

	_gunState.maxBullets = 900;
	_gunState.remainingBullets = _gunState.maxBullets;
	_gunState.maxBulletsInMagazine = 10;
	_gunState.BulletsInMagazine = _gunState.maxBulletsInMagazine;
}

SelectScene::~SelectScene()
{
}

void SelectScene::Update(const Peripheral& p)
{
	(this->*_updater)(p);
}

void SelectScene::Draw()
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0xffffff, true);
	_trimString->ChangeFontSize(50);

	DxLib::DrawString(0, 0, "セレクトシーン", 0xff0000);
	
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);
}
