#include <DxLib.h>
#include "GamePlayingScene.h"
#include "ResultScene.h"
#include "SceneManager.h"
#include "PauseScene.h"
#include "../Peripheral.h"

#include "../Game.h"

#include "../Loader/FileSystem.h"
#include "../Loader/ImageLoader.h"
#include "../Loader/SoundLoader.h"

#include "../Gun.h"


GamePlayingScene::GamePlayingScene()
{
	_pal = 0;
	
	_updater = &GamePlayingScene::FadeinUpdate;
	_drawer = &GamePlayingScene::TestDraw;

	_gun.reset(new Gun());
}

GamePlayingScene::~GamePlayingScene()
{
}

void GamePlayingScene::FadeinUpdate(const Peripheral & p)
{
	if (_pal > 255)
	{
		_pal = 255;
		_updater = &GamePlayingScene::WaitUpdate;
	}
	else
	{
		_pal += 20;
	}
}

void GamePlayingScene::FadeoutUpdate(const Peripheral & p)
{
	if (_pal <= 0)
	{
		SceneManager::Instance().ChangeScene(std::make_unique<ResultScene>());
	}
	else
	{
		_pal -= 20;
	}
}

void GamePlayingScene::WaitUpdate(const Peripheral & p)
{
	/*if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		_updater = &GamePlayingScene::FadeoutUpdate;
	}*/

	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		_gun->Reload();
	}
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		_gun->Shot();
	}
}

void GamePlayingScene::TestDraw()
{
	_gun->Draw();
}

void GamePlayingScene::Update(const Peripheral& p)
{
	(this->*_updater)(p);
}

void GamePlayingScene::Draw()
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0xffffff, true);

	(this->*_drawer)();

	// �t�F�[�h�C��,�A�E�g�̂��߂̖�
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);
}
