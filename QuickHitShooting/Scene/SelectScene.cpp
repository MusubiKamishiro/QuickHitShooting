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

SelectScene::SelectScene() : _dightMax(6)
{
	_pal = 0;
	_trimString = std::make_unique<TrimString>();

	Vector2<int> screen  = Game::Instance().GetScreenSize();
	Vector2<int> btnSize = Vector2<int>(400, 200);

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/gun.png", data);

	int img = data.GetHandle();

	_menu.reset(new Menu());
	_gunState.name		 = "Gun1";
	_gunState.maxBullets = 700;
	_gunState.maxBulletsInMagazine = 10;
	AddGunMenu(_gunState, Vector2<int>(30, 200), Vector2<int>(30 + btnSize.x, 200 + btnSize.y), img);

	_gunState.name		 = "Gun2";
	_gunState.maxBullets = 500;
	_gunState.maxBulletsInMagazine = 10;
	AddGunMenu(_gunState, Vector2<int>(430, 200), Vector2<int>(430 + btnSize.x, 200 + btnSize.y), img);

	_gunState.name		 = "Gun3";
	_gunState.maxBullets = 300;
	_gunState.maxBulletsInMagazine = 40;
	AddGunMenu(_gunState, Vector2<int>(830, 200), Vector2<int>(830 + btnSize.x, 200 + btnSize.y), img);

	btnSize = Vector2<int>(150, 150);
	/// 左矢印ボタンの表示
	Game::Instance().GetFileSystem()->Load("img/leftArrow.png", data);
	img = data.GetHandle();
	AddMenu("left", Vector2<int>(btnSize.x, (screen.y / 2) + btnSize.y), 
				    Vector2<int>((btnSize.x * 2), (screen.y / 2) + (btnSize.y * 2)), img);

	/// 右矢印ボタンの表示
	Game::Instance().GetFileSystem()->Load("img/rightArrow.png", data);
	img = data.GetHandle();
	AddMenu("right", Vector2<int>(screen.x - (btnSize.x * 2), (screen.y / 2) + btnSize.y),
					 Vector2<int>(screen.x - btnSize.x, (screen.y / 2) + (btnSize.y * 2)), img);

	StageInit();

	_updater = &SelectScene::FadeinUpdate;
}

SelectScene::~SelectScene()
{
}

void SelectScene::StageInit()
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
	/// ステージカウントの初期化
	_stageCnt = 0;
	_stageCntMax = stageCnt();

	/// 全ステージの読み込みを行う
	StageData stage;
	_stageDatas.resize(_stageCntMax);
	for (int i = 0; i < _stageCntMax; ++i)
	{
		std::string stagePath = GetStagePath(i + 1);
		Game::Instance().GetFileSystem()->Load(stagePath.c_str(), stage);

		/// ステージデータの登録
		_stageDatas[i] = stage;
	}
}

StageData SelectScene::GetStageData(const int& num)
{
	std::string stagePath = GetStagePath(num + 1);
	StageData stage;
	Game::Instance().GetFileSystem()->Load(stagePath.c_str(), stage);

	return stage;
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
		SceneManager::Instance().ChangeScene(std::make_unique<GamePlayingScene>(_gunState, _stageDatas[_stageCnt]));
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
			_updater = &SelectScene::FadeoutUpdate;
		}
	}

	/// ステージ選択を行うためのメニュー
	if (_menu->CheckClick("right", p))
	{
		_stageCnt = (_stageCnt + 1) % _stageCntMax;
	}
	else if (_menu->CheckClick("left", p))
	{
		_stageCnt = ((_stageCnt + _stageCntMax) - 1) % _stageCntMax;
	}
	else{}

	_menu->Update(p);
}

std::string SelectScene::GetStagePath(const int& num) const
{
	return "StageData/stage" + std::to_string(num) + ".bin";
}

void SelectScene::AddMenu(const std::string& path, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const int& img)
{
	_menu->AddMenuList(path.c_str(), ltPos, rbPos, img);
}

void SelectScene::AddGunMenu(const GunStatus& gunstate, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const int& img)
{
	_menu->AddMenuList(gunstate.name.c_str(), ltPos, rbPos, img);
	_gunStatus.push_back(gunstate);
}

void SelectScene::Update(const Peripheral& p)
{
	(this->*_updater)(p);
}

void SelectScene::Draw()
{
	/// 6桁表示のスコアの文字列を取得するもの
	auto GetScoreDight = [](const int& score, const int& max)
	{
		int divNum = 1;
		int divCnt = 0;
		std::string name;

		/// スコアの桁数を求めている
		while ((score / divNum) >= 1)
		{
			++divCnt;
			divNum *= 10;
		}

		/// スコアが6桁以下の時、0で埋める処理
		if (divCnt <= max)
		{
			for (int i = 0; i < (max - divCnt); ++i)
			{
				name += "0";
			}
		}
		name += std::to_string(score);
		return name;
	};

	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0xffffff, true);

	_menu->Draw();

	_trimString->ChangeFontSize(70);
	std::string text;
	Vector2<int> strSize;

	/// スコアの間隔(debug用)
	int space = 400;
	
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, "000000", strlen("000000"));
	auto score = _stageDatas[_stageCnt].GetStageData().scores;
	for (int i = 0; i < score.size(); ++i)
	{
		text	 = GetScoreDight(score[i], _dightMax);

		DrawString(200 + (space * i) - (strSize.x / 2), 150 - strSize.y / 2,
				  text.c_str(), 0xff0000);
	}

	_trimString->ChangeFontSize(150);
	text = "Stage" + std::to_string(_stageCnt + 1);

	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString(Game::Instance().GetScreenSize().x / 2 - strSize.x / 2, 
			   Game::Instance().GetScreenSize().y - (strSize.y + (strSize.y / 2)),
			   text.c_str(), 0x00ff00);
	
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);


}
