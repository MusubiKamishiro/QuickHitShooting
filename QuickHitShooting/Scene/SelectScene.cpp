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
#include "../Menu.h"


SelectScene::SelectScene()
{
	_pal = 0;
	_trimString = std::make_unique<TrimString>();

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/gun.png", data);
	_img = data.GetHandle();

	_menu.reset(new Menu());
	_gunState.name = "Gun1";
	_gunState.maxBullets = 700;
	_gunState.remainingBullets = _gunState.maxBullets;
	_gunState.maxBulletsInMagazine = 10;
	_gunState.BulletsInMagazine = _gunState.maxBulletsInMagazine;
	AddGunMenu(_gunState, Vector2<int>(300, 600), Size(400, 200), _img);

	_gunState.name = "Gun2";
	_gunState.maxBullets = 500;
	_gunState.remainingBullets = _gunState.maxBullets;
	_gunState.maxBulletsInMagazine = 10;
	_gunState.BulletsInMagazine = _gunState.maxBulletsInMagazine;
	AddGunMenu(_gunState, Vector2<int>(650, 600), Size(400, 200), _img);

	_gunState.name = "Gun3";
	_gunState.maxBullets = 300;
	_gunState.remainingBullets = _gunState.maxBullets;
	_gunState.maxBulletsInMagazine = 40;
	_gunState.BulletsInMagazine = _gunState.maxBulletsInMagazine;
	AddGunMenu(_gunState, Vector2<int>(1000, 600), Size(400, 200), _img);

	_updater = &SelectScene::FadeinUpdate;
}

SelectScene::~SelectScene()
{
}

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
	for (int i = 0; i < _gunStatus.size(); ++i)
	{
		if (_menu->CheckCrick(_gunStatus[i].name.c_str(), p))
		{
			_gunState = _gunStatus[i];
			
			_updater = &SelectScene::FadeoutUpdate;
		}
	}
}

void SelectScene::AddGunMenu(const GunStatus& gunstate, const Vector2<int>& pos, const Size& size, const int& img)
{
	_menu->AddMenuList(gunstate.name.c_str(), pos, size, img);
	_gunStatus.push_back(gunstate);
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
	_menu->Draw();
	
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);
}
