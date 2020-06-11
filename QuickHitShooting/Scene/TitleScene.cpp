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
#include "../SoundPlayer.h"
#include "../TrimString.h"

#include <thread>		// �X���b�h�����ɕK�v

namespace {
	int nowInput = 0;
	int oldInput = 0;
	// ����f�[�^�\���̂̐錾
	// ���������������Ƒ�����f�[�^��������������
	// ��M����f�[�^�\���̂������Ő錾���Ă悢
	StageInfo info = {};
	StageInfo realTimeInfo = {};
}

void TitleScene::FadeinUpdate(const Peripheral & p)
{
	Game::Instance().GetSoundPlayer()->PlaySound("titleBGM", true);

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
		Game::Instance().GetSoundPlayer()->StopSound("titleBGM");
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
		Game::Instance().GetSoundPlayer()->PlaySound("shot");
		_updater = &TitleScene::FadeoutUpdate;
	}
}

void TitleScene::StartDraw()
{
	DxLib::DrawGraph(0, 0, _titleBg, true);
	if ((_sceneTime / 30 % 2) == 0)
	{
		/// �����T�C�Y�ƈʒu���������������B
		_trimString->ChangeFontSize(80);
		DxLib::DrawString(_trimString->GetStringCenterPosx("Click"), _trimString->GetFontSize() + 550, "Click", 0x000000);
	}
}

void TitleScene::SelectPlayDraw()
{
	DxLib::DrawGraph(0, 0, _titleBg, true);
	_trimString->ChangeFontSize(50);
		
	DxLib::DrawString(_trimString->GetStringCenterPosx(" 1P vs CPU"), 500, " 1P vs CPU", 0x000000);
	DxLib::DrawString(_trimString->GetStringCenterPosx(" 1P vs 2P "), _trimString->GetFontSize() + 500 + 10, " 1P vs 2P ", 0x000000);
}

TitleScene::TitleScene()
{
	_pal = 0;
	_trimString = std::make_unique<TrimString>();
	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/title.png", data);
	_titleBg = data.GetHandle();

	_updater = &TitleScene::FadeinUpdate;
	_drawer  = &TitleScene::StartDraw;

	SoundData sdata;
	Game::Instance().GetFileSystem()->Load("sound/bgm/title.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("titleBGM", sdata.GetHandle(), 40);
	/// �^�C�g����X�e�[�W�I���̎��ɃV���b�g�̌��ʉ����g�p����̂ňړ������B��
	Game::Instance().GetFileSystem()->Load("sound/se/handgun-firing.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("shot", sdata.GetHandle(), 80);	


	//##############################################################
	// ���A���^�C���T�[�o�[�X���b�h
	/*std::thread RealSendThread([]() {
		NetWorkWS2::Instance().Initialize("192.168.11.47");
		NetWorkWS2::Instance().RealTimeServer(realTimeInfo);
		});
	RealSendThread.detach();*/
	//##############################################################

	//##############################################################
	// ���A���^�C���N���C�A���g�X���b�h
	/*std::thread RealReciveThread([]() {
		NetWorkWS2::Instance().Initialize("192.168.11.55");
		NetWorkWS2::Instance().RealTimeClient(realTimeInfo);
		});
	RealReciveThread.detach();*/
	//##############################################################
}


TitleScene::~TitleScene()
{
	NetWorkWS2::Instance().Terminate();
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
	// �l�b�g���[�N�ʐM�Ăяo���i���j(�T�[���@�[)
	
	if (nowInput && !oldInput) {
		std::thread reciveThread([]() {
			//######################################################
			// StageInfo�e�X�g�f�[�^
			std::array<std::string, 3> names = { "Title","Game","Result" };
			info.names = names;
			TargetData tdata = {};
			tdata.pos = { 100,500 };
			tdata.banishTime = 600;
			tdata.dispTime = 1000;
			tdata.type = 1;
			vec_target vtarget = {tdata};
			info.targetData.push_back(vtarget);
			info.scores = { 30,50,60 };
			//######################################################
			NetWorkWS2::Instance().Initialize("192.168.11.47");
			NetWorkWS2::Instance().SendServer(info);
			});
		reciveThread.detach();
	}
	oldInput = nowInput;
	nowInput = CheckHitKey(KEY_INPUT_S);
	//##############################################################

	//##############################################################
	// �l�b�g���[�N�ʐM�Ăяo���i���j(�N���@�C�A���g)
	/*if (nowInput && !oldInput) {
		std::thread reciveThread([]() {
			//######################################################
			// StageInfo�e�X�g�f�[�^
			StageInfo info = {};
			//######################################################
			NetWorkWS2::Instance().Initialize("192.168.11.55");
			NetWorkWS2::Instance().RecivedClient(info);
			});
		reciveThread.detach();
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
