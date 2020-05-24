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
	int img = data.GetHandle();

	_menu.reset(new Menu());
	_gunState.name = "Gun1";
	_gunState.maxBullets = 700;
	_gunState.maxBulletsInMagazine = 10;
	AddGunMenu(_gunState, Vector2<int>(150, 500), Vector2<int>(300, 700), img);

	_gunState.name = "Gun2";
	_gunState.maxBullets = 500;
	_gunState.maxBulletsInMagazine = 10;
	AddGunMenu(_gunState, Vector2<int>(400, 500), Vector2<int>(550, 700), img);

	_gunState.name = "Gun3";
	_gunState.maxBullets = 300;
	_gunState.maxBulletsInMagazine = 40;
	AddGunMenu(_gunState, Vector2<int>(650, 500), Vector2<int>(800, 700), img);

	CreateStageMenu();

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
		SceneManager::Instance().ChangeScene(std::make_unique<GamePlayingScene>(_gunState, _stageData));
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
		if (_menu->CheckClick(_gunStatus[i].name.c_str(), p))
		{
			_gunState = _gunStatus[i];
			
			// _updater = &SelectScene::FadeoutUpdate;
		}
	}

	std::string stagePath;
	for (int i = 0; i < _stageDatas.size(); ++i)
	{
		/// 押したメニューボタンのステージ情報を取得
		stagePath = GetStagePath(i + 1);
		if (_menu->CheckClick(stagePath.c_str(), p))
		{
			_stageData = _stageDatas[i];
			_updater = &SelectScene::FadeoutUpdate;
		}
	}
	_menu->Update(p);
}


void SelectScene::CreateStageMenu()
{
	/// フォルダーの中にあるステージ数を取得するもの
	auto stageCnt = []()
	{
		/// 調べるステージハンドルの取得
		HANDLE handle;
		WIN32_FIND_DATA findData;
		std::string searchName = "StageData/*.bin";
		handle = FindFirstFile(searchName.c_str(), &findData);
		int cnt = 0;

		do {
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				/// ステージ数を加算させる
				cnt++;
			}
		} while (FindNextFile(handle, &findData));

		/// 開いたフォルダーを閉じる
		FindClose(handle);

		/// ステージデータが読み込めなかった時に入る
		if (cnt == 0)
		{
			MessageBox(GetMainWindowHandle(),
				"ステージデータが見つかりませんでした。",
				"Not Found StageData",
				MB_OK);
		}
		return cnt;
	};

	/// 仮のステージメニューの画像取得
	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/StageBox.png", data);
	int img = data.GetHandle();

	/// メニュー用の座標設定に使用するもの
	Vector2<int> menuPos;
	Vector2<int> menuSize	= Vector2<int>(180, 180);
	Vector2<int> menuOffset = Vector2<int>(100, 100);

	int cnt = stageCnt();
	StageData stage;
	for (int i = 0; i < cnt; ++i)
	{
		/// ステージデータ用のメニューボックスの生成
		std::string stagePath = GetStagePath(i + 1);
		Game::Instance().GetFileSystem()->Load(stagePath.c_str(), stage);
		_stageDatas.push_back(stage);

		/// メニューの座標設定
		menuPos.x = menuOffset.x + ((i % 5) * 40) + ((i % 5) * menuSize.x);
		menuPos.y = menuOffset.y + ((i / 5) * 40) + ((i / 5) * menuSize.y);

		/// メニューの追加
		AddStageMenu(stagePath.c_str(), menuPos, menuPos + menuSize, img);
	}
}

std::string SelectScene::GetStagePath(const int& num) const
{
	return "StageData/stage" + std::to_string(num) + ".bin";
}


void SelectScene::AddGunMenu(const GunStatus& gunstate, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const int& img)
{
	_menu->AddMenuList(gunstate.name.c_str(), ltPos, rbPos, img);
	_gunStatus.push_back(gunstate);
}

void SelectScene::AddStageMenu(const std::string& stageName, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const int& img)
{
	_menu->AddMenuList(stageName.c_str(), ltPos, rbPos, img);
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
