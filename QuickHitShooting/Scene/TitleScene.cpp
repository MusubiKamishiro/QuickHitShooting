#include "../NetWorkWS2.h"		// �l�b�g���[�N�N���X�@DxLib����ɌĂяo���K�v����
#include <DxLib.h>
#include "TitleScene.h"
#include "SelectScene.h"
#include "SceneManager.h"
#include "../Peripheral.h"
#include "../Game.h"

#include "../Loader/FileSystem.h"
#include "../Loader/ImageLoader.h"
#include "../Loader/SoundLoader.h"
#include "../TrimString.h"

#include <thread>		// �X���b�h�����ɕK�v

namespace {
	int nowInput = 0;
	int oldInput = 0;
}

void TitleScene::FadeinUpdate(const Peripheral & p)
{
	if (_pal >= 255)
	{
		_pal = 255;
		_updater = &TitleScene::WaitUpdate;
	}
	else
	{
		_pal += 20;
	}
}

void TitleScene::FadeoutUpdate(const Peripheral & p)
{
	if (_pal <= 0)
	{
		SceneManager::Instance().ChangeScene(std::make_unique <SelectScene>());
	}
	else
	{
		_pal -= 20;
	}
}

void TitleScene::WaitUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		_updater = &TitleScene::FadeoutUpdate;
	}
}

void TitleScene::StartDraw()
{
	if ((_sceneTime / 30 % 2) == 0)
	{
		_trimString->ChangeFontSize(50);
		DxLib::DrawString(_trimString->GetStringCenterPosx("Click"), _trimString->GetFontSize() + 500, "Click", 0x000000);
	}
}

void TitleScene::SelectPlayDraw()
{
	_trimString->ChangeFontSize(50);
		
	DxLib::DrawString(_trimString->GetStringCenterPosx(" 1P vs CPU"), 500, " 1P vs CPU", 0x000000);
	DxLib::DrawString(_trimString->GetStringCenterPosx(" 1P vs 2P "), _trimString->GetFontSize() + 500 + 10, " 1P vs 2P ", 0x000000);
}

TitleScene::TitleScene()
{
	_pal = 0;
	_trimString = std::make_unique<TrimString>();

	_updater = &TitleScene::FadeinUpdate;
	_drawer = &TitleScene::StartDraw;
	
	//##############################################################
	// ���A���^�C���T�[�o�[�X���b�h
	std::thread RealSendThread([]() {
		SendDataWS2 dataws2 = {};
		dataws2.Buffer = "KUSOZAKO";
		NetWorkWS2::Instance().Initialize("192.168.11.47");
		NetWorkWS2::Instance().RealTimeServer(dataws2);
		});
	RealSendThread.detach();
	//##############################################################

	//##############################################################
	// ���A���^�C���N���C�A���g�X���b�h
	/*std::thread RealReciveThread([]() {
		SendDataWS2 dataws2 = {};
		dataws2.Buffer = "";
		NetWorkWS2::Instance().Initialize("192.168.11.55");
		NetWorkWS2::Instance().RealTimeClient(dataws2);
		});
	RealReciveThread.detach();*/
	//##############################################################
}


TitleScene::~TitleScene()
{
}

void TitleScene::Update(const Peripheral& p)
{
	//##############################################################
	// ���͏����X���b�h����
	std::thread updateThread([&]() {
		(this->*_updater)(p);
		});
	updateThread.join();
	//##############################################################

	//##############################################################
	// �l�b�g���[�N�ʐM�Ăяo���i���j(�T�[�o�[)
	if (nowInput && !oldInput) {
		std::thread reciveThread([]() {
			SendDataWS2 dataws2 = {};
			dataws2.Buffer = "KUSOZAKO";
			NetWorkWS2::Instance().Initialize("192.168.11.47");
			NetWorkWS2::Instance().SendServer(dataws2);
			});
		reciveThread.detach();
	}
	oldInput = nowInput;
	nowInput = CheckHitKey(KEY_INPUT_S);
	//##############################################################

	//##############################################################
	// �l�b�g���[�N�ʐM�Ăяo���i���j(�N���C�A���g)
	/*if (nowInput && !oldInput) {
		SendDataWS2 dataws2 = {};
		dataws2.Buffer = "";
		std::thread sendThread([&]() {
			NetWorkWS2::Instance().Initialize("192.168.11.55");
			NetWorkWS2::Instance().RecivedClient(dataws2);
			});
		sendThread.detach();
	}
	oldInput = nowInput;
	nowInput = CheckHitKey(KEY_INPUT_S);*/
	//##############################################################
}

void TitleScene::Draw()
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0xffffff, true);
	//#########################################
	// �`�揈���X���b�h����
	std::thread drawThread([&]() {
		(this->*_drawer)();
		});
	drawThread.join();
	//########################################
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);
}
