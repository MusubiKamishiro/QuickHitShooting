#include "Game.h"
#include <DxLib.h>
#include "Peripheral.h"
#include "Scene/SceneManager.h"
#include "Loader/FileSystem.h"
#include "FrameFixity/FrameFixity.h"
#include "NetWork.h"

/// デバッグ用のインクルード
#include "Loader/StageLoader.h"

namespace {
	std::vector<int> ip = { 192,168,56,1 };
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
//#ifdef _DEBUG
//	DxLib::ChangeWindowMode(true);
//#else
//	int ans = MessageBox(DxLib::GetMainWindowHandle(), "フルスクリーンで表示しますか？", "画面の大きさどうしようか", MB_YESNO | MB_ICONQUESTION);
//
//	if (ans == IDYES)
//	{
//		DxLib::ChangeWindowMode(false);
//	}
//	else
//	{
//		DxLib::ChangeWindowMode(true);
//	}
//#endif // _DEBUG

	DxLib::ChangeWindowMode(true);

	// 画面サイズの設定
	DxLib::SetGraphMode(_screenSize.x, _screenSize.y, 32);

	// DxLibの初期化
	if (DxLib::DxLib_Init() == -1)
	{
		return;
	}

	DxLib::SetMainWindowText("QuickHitGame");	// タイトル
	//DxLib::SetWindowIconID(IDI_ICON1);			// アイコン
	DxLib::SetDrawScreen(DX_SCREEN_BACK);		// 裏画面に描画

	// フォントの変更
	//AddFontResourceEx("fonts/PixelMplus10-Regular.ttf", FR_PRIVATE, nullptr);
	//DxLib::ChangeFont("PixelMplus10", DX_CHARSET_DEFAULT);
	

	_peripheral.reset(new Peripheral());
	_fileSystem.reset(new FileSystem());
	// NetWork::Instance().Connect(ip);

	auto stageCnt = []()
	{
		int cnt = 0;
		HANDLE handle;
		WIN32_FIND_DATA findData;
		std::string searchName = "../StageData/*.bin";
		handle = FindFirstFile(searchName.c_str(), &findData);

		do {
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				cnt++;
			}
		} while (FindNextFile(handle, &findData));
		FindClose(handle);

		if (cnt == 0)
		{
			MessageBox(GetMainWindowHandle(),
				"ステージデータが見つかりませんでした。",
				"Not Found StageData",
				MB_OK);
		}
		return cnt;
	};

	int cnt = stageCnt();

	StageData stage;
	for (int i = 0; i < cnt; ++i)
	{
		TargetData debug;
		std::string stageNum = std::to_string(i + 1);
		_fileSystem->Load("StageData/stage" + stageNum + ".bin", stage);

		for (auto wave : stage.GetStageData())
		{
			for (auto target : wave)
			{
				debug = target;
			}
		}
	}

}

void Game::Run()
{
	auto& scenes = SceneManager::Instance();
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


#ifdef _DEBUG
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
			_peripheral->DebugDraw();
#endif // _DEBUG
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

const Vector2<int>& Game::GetScreenSize()const
{
	return _screenSize;
}
