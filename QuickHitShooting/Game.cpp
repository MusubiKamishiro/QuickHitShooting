#include "Game.h"
#include <DxLib.h>
#include "Peripheral.h"
#include "Scene/SceneManager.h"
#include "Loader/FileSystem.h"
#include "FrameFixity/FrameFixity.h"
#include "NetWork.h"

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
#ifdef _DEBUG
	DxLib::ChangeWindowMode(true);
#else
	int ans = MessageBox(DxLib::GetMainWindowHandle(), "�t���X�N���[���ŕ\�����܂����H", "��ʂ̑傫���ǂ����悤��", MB_YESNO | MB_ICONQUESTION);

	if (ans == IDYES)
	{
		DxLib::ChangeWindowMode(false);
	}
	else
	{
		DxLib::ChangeWindowMode(true);
	}
#endif // _DEBUG

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
	NetWork::Instance().Connect(ip);
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