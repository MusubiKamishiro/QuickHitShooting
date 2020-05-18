#include "PauseScene.h"
#include <DxLib.h>
#include "../Peripheral.h"
#include "SceneManager.h"
#include "SelectScene.h"

#include "../Game.h"
#include "../Menu.h"
#include "../Loader/FileSystem.h"
#include "../Loader/ImageLoader.h"


PauseScene::PauseScene()
{
	_pal = 255;
	_updater = &PauseScene::FadeinUpdate;

	_menu.reset(new Menu());
	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/sample01.png", data);
	int i = data.GetHandle();
	_menu->AddMenuList("BackGame", Vector2<int>(_scrSize.x/2, 150), Size(300, 150), i);
	Game::Instance().GetFileSystem()->Load("img/sample02.png", data);
	i = data.GetHandle();
	_menu->AddMenuList("ReTry", Vector2<int>(_scrSize.x / 2, 400), Size(300, 150), i);
	Game::Instance().GetFileSystem()->Load("img/sample03.png", data);
	i = data.GetHandle();
	_menu->AddMenuList("BackSelect", Vector2<int>(_scrSize.x / 2, 650), Size(300, 150), i);
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
	if (_menu->CheckCrick("BackGame", p))
	{
		SceneManager::Instance().PopScene();
	}
	else if (_menu->CheckCrick("ReTry", p))
	{
		
	}
	else if (_menu->CheckCrick("BackSelect", p))
	{
		_updater = &PauseScene::FadeoutUpdate;
	}
}

void PauseScene::Update(const Peripheral& p)
{
	(this->*_updater)(p);
}

void PauseScene::Draw()
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
	
	DxLib::DrawString(100, 100, "�|�[�Y��", 0x0000ff);
	_menu->Draw();

	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);
}
