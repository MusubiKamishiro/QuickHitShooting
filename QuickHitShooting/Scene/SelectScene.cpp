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
#include "../Loader/SoundLoader.h"
#include "../SoundPlayer.h"
#include "../Menu.h"
#include <thread>

#include "EffekseerForDXLib.h"

SelectScene::SelectScene() : _dightMax(6)
{
	_pal = 0;
	_trimString = std::make_unique<TrimString>();

	ImageData data;
	_menu.reset(new Menu());

	/// セレクトシーンの初期化
	ButtonInit(data);
	GunInit(data);
	StageInit();
	SoundInit();

	_updater = &SelectScene::FadeinUpdate;
}

SelectScene::~SelectScene()
{
}

///銃の初期化
void SelectScene::GunInit(ImageData& data)
{
	int space	 = 95;
	Size btnSize = Size(300, 150);
	
	/* 銃の登録 */

	/// 左端の銃
	Game::Instance().GetFileSystem()->Load("img/gun1.png", data);
	_gunState.name = "Gun1";
	_gunState.maxBullets = 700;
	_gunState.maxBulletsInMagazine = 10;
	AddGunMenu(_gunState, Vector2<int>(space, _scrSize.y - btnSize.height - 30),
						  Vector2<int>(space + btnSize.width, _scrSize.y - 30), data.GetHandle());

	/// 真ん中の銃
	Game::Instance().GetFileSystem()->Load("img/gun2.png", data);
	_gunState.name		 = "Gun2";
	_gunState.maxBullets = 500;
	_gunState.maxBulletsInMagazine = 10;
	AddGunMenu(_gunState, Vector2<int>((_scrSize.x / 2) - (btnSize.width / 2), _scrSize.y - btnSize.height - 30),
						  Vector2<int>((_scrSize.x / 2) + (btnSize.width / 2), _scrSize.y - 30), data.GetHandle());

	/// 右端の銃
	Game::Instance().GetFileSystem()->Load("img/gun3.png", data);
	_gunState.name		 = "Gun3";
	_gunState.maxBullets = 300;
	_gunState.maxBulletsInMagazine = 40;
	AddGunMenu(_gunState, Vector2<int>(_scrSize.x - space - btnSize.width, _scrSize.y - btnSize.height - 30),
						  Vector2<int>(_scrSize.x - space, _scrSize.y - 30), data.GetHandle());
}

/// ボタンの初期化
void SelectScene::ButtonInit(ImageData& data)
{
	/// スクリーンサイズの取得
	Size btnSize = Size(300, 150);
	int img;

	/// 下地画像の取得
	Game::Instance().GetFileSystem()->Load("img/plate/rankBd.png", data);
	_rankBd = data.GetHandle();

	Game::Instance().GetFileSystem()->Load("img/plate/gunBd.png", data);
	_gunBd = data.GetHandle();

	Game::Instance().GetFileSystem()->Load("img/plate/stageBd.png", data);
	_stageBd = data.GetHandle();

	/// 背景画像の取得
	Game::Instance().GetFileSystem()->Load("img/select.png", data);
	_selectBg = data.GetHandle();

	btnSize = Vector2<int>(150, 150);
	/// 左矢印ボタンの表示
	Game::Instance().GetFileSystem()->Load("img/leftArrow.png", data);
	img = data.GetHandle();
	_menu->AddMenuList("left", Vector2<int>((btnSize.width / 2), (_scrSize.y / 2) - (btnSize.height / 2)),
							   Vector2<int>(btnSize.width + (btnSize.width / 2), (_scrSize.y / 2) + (btnSize.height / 2)), img);

	/// 右矢印ボタンの表示
	Game::Instance().GetFileSystem()->Load("img/rightArrow.png", data);
	img = data.GetHandle();
	_menu->AddMenuList("right", Vector2<int>(_scrSize.x - (btnSize.width / 2), (_scrSize.y / 2) + (btnSize.height / 2)),
								Vector2<int>(_scrSize.x - btnSize.width - (btnSize.width / 2), (_scrSize.y / 2) - (btnSize.height / 2)), img);

	Game::Instance().GetFileSystem()->Load("img/stageboard.png", data);
}

/// 音の初期化
void SelectScene::SoundInit()
{
	SoundData sdata;
	Game::Instance().GetFileSystem()->Load("sound/bgm/select.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("selectBGM", sdata.GetHandle(), 35);

	Game::Instance().GetFileSystem()->Load("sound/se/selectstg.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("selectstg", sdata.GetHandle(), 50);
}
/// ステージの初期化
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
	_stageCnt	 = 0;
	_stageCntMax = stageCnt();

	StageData stage;
	Game::Instance().GetFileSystem()->Load(GetStagePath(_stageCnt + 1).c_str(), stage);
	_scores = stage.GetStageData().scores;
	_names  = stage.GetStageData().names;
}

/// ステージデータの取得用
StageData SelectScene::GetStageData(const int& num)
{
	std::string stagePath = GetStagePath(num + 1);
	StageData stage;
	Game::Instance().GetFileSystem()->Load(stagePath.c_str(), stage);

	return stage;
}

/// フェードイン処理の更新用
void SelectScene::FadeinUpdate(const Peripheral& p)
{
	Game::Instance().GetSoundPlayer()->PlaySound("selectBGM", true);

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

/// フェードアウト処理の更新用
void SelectScene::FadeoutUpdate(const Peripheral& p)
{
	if (_pal <= 0)
	{
		Game::Instance().GetSoundPlayer()->StopSound("selectBGM");
		/// ステージデータの読み込み
		SceneManager::Instance().ChangeScene(std::make_unique<GamePlayingScene>(_gunState, GetStagePath(_stageCnt + 1)));
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
			Game::Instance().GetSoundPlayer()->PlaySound("shot");
			_gunState = _gunStatus[i];
			_updater  = &SelectScene::FadeoutUpdate;
		}
	}

	/// エフェクトの再生を行う
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		/// エフェクトの再生
		_playEffect = PlayEffekseer2DEffect(_shotEffect);

		_efkPos = p.GetMousePos();
	}

	/// ステージ選択を行うためのメニュー
	if (_menu->CheckClick("right", p))
	{
		Game::Instance().GetSoundPlayer()->PlaySound("selectstg");
		_stageCnt = (_stageCnt + 1) % _stageCntMax;

		/// スコアとランキング名の更新を行う
		StageData stage;
		Game::Instance().GetFileSystem()->Load(GetStagePath(_stageCnt + 1).c_str(), stage);
		_scores = stage.GetStageData().scores;
		_names  = stage.GetStageData().names;
	}
	else if (_menu->CheckClick("left", p))
	{
		Game::Instance().GetSoundPlayer()->PlaySound("selectstg");
		_stageCnt = ((_stageCnt + _stageCntMax) - 1) % _stageCntMax;

		/// スコアとランキング名の更新を行う
		StageData stage;
		Game::Instance().GetFileSystem()->Load(GetStagePath(_stageCnt + 1).c_str(), stage);
		_scores = stage.GetStageData().scores;
		_names  = stage.GetStageData().names;
	}
	else{}

	std::thread menuUpdateThread([&]() {
		_menu->Update(p);
		});
	menuUpdateThread.join();
}

/// ステージパスの取得用
std::string SelectScene::GetStagePath(const int& num) const
{
	return "StageData/stage" + std::to_string(num) + ".bin";
}

/// 銃のメニュー追加用
void SelectScene::AddGunMenu(const GunStatus& gunstate, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const int& img)
{
	_menu->AddMenuList(gunstate.name.c_str(), ltPos, rbPos, img);
	_gunStatus.push_back(gunstate);
}

void SelectScene::Update(const Peripheral& p)
{
	std::thread updateThread([&]() {
		(this->*_updater)(p);
		});
	updateThread.join();
}

/// 描画用
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
		if (divCnt < max)
		{
			for (int i = 0; i < (max - divCnt); ++i)
			{
				name += "0";
			}
		}
		if (score > 0)
		{
			name += std::to_string(score);
		}
		
		return name;
	};
	/* 描画開始位置 */

	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0xffffff, true);

	/// 背景の描画
	DxLib::DrawGraph(0, 0, _selectBg, true);

	/// 下地の描画
	DxLib::DrawGraph(40, 0, _rankBd, true);
	DxLib::DrawGraph((_scrSize.x / 3) - (_scrSize.x / 8), 
					 _scrSize.y / 2 - 110, _stageBd, true);
	DxLib::DrawGraph(40, _scrSize.y - 230, _gunBd, true);

	/// メニューの描画
	_menu->Draw();

	_trimString->ChangeFontSize(70);
	std::string text;
	Vector2<int> strSize;

	/// スコアの間隔
	int space = 400;

	/// スコアの描画
	for (int i = 0; i < _scores.size(); ++i)
	{
		text = GetScoreDight(_scores[i], _dightMax);
		GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(),
			strlen(text.c_str()));
		DrawString(240 + (space * i) - (strSize.x / 2), (strSize.y / 2) + (strSize.y / 3),
				   text.c_str(), 0x000000);
	}

	/// プレイヤーランキングの描画
	text = std::to_string(0) + ". " + "AAA";
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	for (int i = 0; i < _names.size(); ++i)
	{
		text = std::to_string(i + 1) + ". " + _names[i];
		DrawString(240 + (space * i) - (strSize.x / 2), (strSize.y / 4),
				  text.c_str(), 0xff0000);
	}

	/// ステージの描画
	_trimString->ChangeFontSize(120);
	text = "STAGE " + std::to_string(_stageCnt + 1);

	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_scrSize.x / 2) - (_scrSize.x / 5),  (_scrSize.y / 2) - (strSize.y / 3),
			   text.c_str(), 0x000080);

	/// エフェクトの描画
	DrawEffect();

	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);


}
