#include "Game.h"
#include "NetWorkWS2.h"
#include <DxLib.h>
#include "Peripheral.h"
#include "Scene/SceneManager.h"
#include "Loader/FileSystem.h"
#include "SoundPlayer.h"
#include "FrameFixity/FrameFixity.h"
#include <iostream>
#include <thread>
#include "resource.h"

#include "Loader/StageLoader.h"
#include "Loader/EffekseerLoader.h"

/// Effekseerを使用するためのヘッダー
#include "EffekseerForDXLib.h"

namespace {
	int nowInput = 0;
	int oldInput = 0;
}

Game::Game() : _screenSize(1280, 720)
{
}

void Game::operator=(const Game &)
{
}

Game::~Game()
{
}

void Game::Initialize()
{
#ifdef _DEBUG
	DxLib::ChangeWindowMode(true);
#else
	/*int ans = MessageBox(DxLib::GetMainWindowHandle(), "フルスクリーンで表示しますか？", "画面の大きさどうしようか", MB_YESNO | MB_ICONQUESTION);

	if (ans == IDYES)
	{
		DxLib::ChangeWindowMode(false);
	}
	else
	{
		DxLib::ChangeWindowMode(true);
	}*/
	DxLib::ChangeWindowMode(false);
#endif // _DEBUG

	// 画面サイズの設定
	DxLib::SetGraphMode(_screenSize.x, _screenSize.y, 32);

	/// effekseerを使用するための初期設定(DirectX11を使用する)
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// DxLibの初期化
	if (DxLib::DxLib_Init() == -1)
	{
		return;
	}
	

	DxLib::SetMainWindowText("QuickHitGame");	// タイトル
	DxLib::SetWindowIconID(IDI_ICON1);			// アイコン
	DxLib::SetDrawScreen(DX_SCREEN_BACK);		// 裏画面に描画

	// Effekseerの初期化(引数にパーティクルの最大生成数を指定)
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return;
	}
	// Effekseerを使用する時の設定
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	// Zバッファの有効化(2DゲームでもZバッファを使用する)		◆
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みの有効化(2DゲームでもZバッファを使用する)
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。		◆
	SetWriteZBuffer3D(true);

	// Effekseerに2D描画の設定をする。　◆
	Effekseer_Set2DSetting(_screenSize.x, _screenSize.y);

	// フォントの変更
	if (!AddFontResourceEx("Font/font.ttf", FR_PRIVATE, nullptr))
	{
		MessageBox(NULL, "フォント読込み失敗", "", MB_OK);
	}

	DxLib::ChangeFont("Edmunds Distressed", DX_CHARSET_DEFAULT);
	
	_fileSystem.reset(new FileSystem());
	_soundPlayer.reset(new SoundPlayer());
	_peripheral.reset(new Peripheral());
}

void Game::Run()
{
	auto& scenes	= SceneManager::Instance();
	FrameFixity& ff = FrameFixity::Instance();
	ff.FFInitialize();

	while (DxLib::ProcessMessage() == 0)
	{
		if (ff.CheckReceiveMessage())
		{
			if (ff.GetReceiveMessage().message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			ff.AdjustmentFrameLate();

			DxLib::ClearDrawScreen();

			// エスケープキーで終了
			if (DxLib::CheckHitKey(KEY_INPUT_ESCAPE))
			{
				break;
			}

			_peripheral->Update();
			scenes.Update(*_peripheral);
			scenes.Draw();

			/*std::thread updateThread([&]() {
				_peripheral->Update();
				scenes.Update(*_peripheral);
				scenes.Draw();
				});
			updateThread.join();*/

			/*if (nowInput && !oldInput) {
				std::thread reciveThread([]() {
					DxLib::DxLib_Init();
					SendDataWS2 dataws2 = {};
					dataws2.Buffer = "KUSOZAKO";
					NetWorkWS2::Instance().Initialize("192.168.11.47");
					NetWorkWS2::Instance().SendServer(dataws2);
					});
				reciveThread.detach();
			}

			oldInput = nowInput;
			nowInput = CheckHitKey(KEY_INPUT_S);*/

#ifdef _DEBUG
			
			
#endif // _DEBUG
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
			_peripheral->Draw();
			DxLib::ScreenFlip();
		}
	}

	ff.Terminate();
}

void Game::Terminate()
{
	DxLib::DxLib_End();
}

const std::shared_ptr<FileSystem> Game::GetFileSystem() const
{
	return _fileSystem;
}

const std::shared_ptr<SoundPlayer> Game::GetSoundPlayer() const
{
	return _soundPlayer;
}

const Vector2<int>& Game::GetScreenSize()const
{
	return _screenSize;
}
