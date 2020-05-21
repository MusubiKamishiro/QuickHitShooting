#include "Game.h"
#include "NetWorkWS2.h"
#include <DxLib.h>
#include "Peripheral.h"
#include "Scene/SceneManager.h"
#include "Loader/FileSystem.h"
#include "FrameFixity/FrameFixity.h"
#include <iostream>
#include <thread>

#include "Loader/StageLoader.h"

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
//#ifdef _DEBUG
//	DxLib::ChangeWindowMode(true);
//#else
//	int ans = MessageBox(DxLib::GetMainWindowHandle(), "�t���X�N���[���ŕ\�����܂����H", "��ʂ̑傫���ǂ����悤��", MB_YESNO | MB_ICONQUESTION);
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

	// ��ʃT�C�Y�̐ݒ�
	DxLib::SetGraphMode(_screenSize.x, _screenSize.y, 32);

	// DxLib�̏�����
	if (DxLib::DxLib_Init() == -1)
	{
		return;
	}

	DxLib::SetMainWindowText("QuickHitGame");	// �^�C�g��
	//DxLib::SetWindowIconID(IDI_ICON1);			// �A�C�R��
	DxLib::SetDrawScreen(DX_SCREEN_BACK);		// ����ʂɕ`��

	// �t�H���g�̕ύX
	//AddFontResourceEx("fonts/PixelMplus10-Regular.ttf", FR_PRIVATE, nullptr);
	//DxLib::ChangeFont("PixelMplus10", DX_CHARSET_DEFAULT);
	

	_peripheral.reset(new Peripheral());
	_fileSystem.reset(new FileSystem());
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

			// �G�X�P�[�v�L�[�ŏI��
			if (DxLib::CheckHitKey(KEY_INPUT_ESCAPE))
			{
				break;
			}

			std::thread updateThread([&]() {
				_peripheral->Update();
				scenes.Update(*_peripheral);
				scenes.Draw();
				});
			updateThread.join();

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
