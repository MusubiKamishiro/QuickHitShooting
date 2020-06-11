#include "PauseScene.h"
#include <DxLib.h>
#include "../Peripheral.h"
#include "SceneManager.h"
#include "SelectScene.h"

#include "../Game.h"
#include "../Menu.h"
#include "../SoundPlayer.h"
#include "../Loader/FileSystem.h"
#include "../Loader/ImageLoader.h"


PauseScene::PauseScene()
{
	_pal = 255;
	_updater = &PauseScene::FadeinUpdate;

	_menu.reset(new Menu());
	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/button/continue.png", data);
	int i = data.GetHandle();
	_menu->AddMenuList("BackGame", Vector2<int>(_scrSize.x/2 - 150, 50), Vector2<int>(_scrSize.x/2 + 150, 180), i);
	Game::Instance().GetFileSystem()->Load("img/button/retry.png", data);
	i = data.GetHandle();
	_menu->AddMenuList("ReTry", Vector2<int>(_scrSize.x/2 - 150, 230), Vector2<int>(_scrSize.x/2 + 150, 360), i);
	Game::Instance().GetFileSystem()->Load("img/button/backselect.png", data);
	i = data.GetHandle();
	_menu->AddMenuList("BackSelect", Vector2<int>(_scrSize.x/2 - 150, 410), Vector2<int>(_scrSize.x/2 + 150, 540), i);
}

PauseScene::~PauseScene()
{
}

void PauseScene::FadeinUpdate(const Peripheral & p)
{
	_pal = 255;
	_updater = &PauseScene::WaitUpdate;
}

void PauseScene::FadeoutUpdate(const Peripheral & p)
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

void PauseScene::WaitUpdate(const Peripheral & p)
{
	if (_menu->CheckClick("BackGame", p))
	{
		Game::Instance().GetSoundPlayer()->PlaySound("pause");
		SceneManager::Instance().PopScene();
	}
	else if (_menu->CheckClick("ReTry", p))
	{
		
	}
	else if (_menu->CheckClick("BackSelect", p))
	{
		Game::Instance().GetSoundPlayer()->PlaySound("shot");
		Game::Instance().GetSoundPlayer()->StopSound("gameBGM");
		_updater = &PauseScene::FadeoutUpdate;
	}
}

void PauseScene::Update(const Peripheral& p)
{
	_menu->Update(p);

	(this->*_updater)(p);
}

void PauseScene::Draw()
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
	
	DxLib::DrawString(100, 100, "ƒ|[ƒY’†", 0x0000ff);
	_menu->Draw();

	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);
}
