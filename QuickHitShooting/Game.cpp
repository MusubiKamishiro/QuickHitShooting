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

/// Effekseer���g�p���邽�߂̃w�b�_�[
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
	/*int ans = MessageBox(DxLib::GetMainWindowHandle(), "�t���X�N���[���ŕ\�����܂����H", "��ʂ̑傫���ǂ����悤��", MB_YESNO | MB_ICONQUESTION);

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

	// ��ʃT�C�Y�̐ݒ�
	DxLib::SetGraphMode(_screenSize.x, _screenSize.y, 32);

	/// effekseer���g�p���邽�߂̏����ݒ�(DirectX11���g�p����)
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// DxLib�̏�����
	if (DxLib::DxLib_Init() == -1)
	{
		return;
	}
	

	DxLib::SetMainWindowText("QuickHitGame");	// �^�C�g��
	DxLib::SetWindowIconID(IDI_ICON1);			// �A�C�R��
	DxLib::SetDrawScreen(DX_SCREEN_BACK);		// ����ʂɕ`��

	// Effekseer�̏�����(�����Ƀp�[�e�B�N���̍ő吶�������w��)
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return;
	}
	// Effekseer���g�p���鎞�̐ݒ�
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	// Z�o�b�t�@�̗L����(2D�Q�[���ł�Z�o�b�t�@���g�p����)		��
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂̗L����(2D�Q�[���ł�Z�o�b�t�@���g�p����)
	// Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B		��
	SetWriteZBuffer3D(true);

	// Effekseer��2D�`��̐ݒ������B�@��
	Effekseer_Set2DSetting(_screenSize.x, _screenSize.y);

	// �t�H���g�̕ύX
	if (!AddFontResourceEx("Font/font.ttf", FR_PRIVATE, nullptr))
	{
		MessageBox(NULL, "�t�H���g�Ǎ��ݎ��s", "", MB_OK);
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

			// �G�X�P�[�v�L�[�ŏI��
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
