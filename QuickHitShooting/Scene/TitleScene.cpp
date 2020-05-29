#include "../NetWorkWS2.h"		// ネットワーククラス　DxLibより先に呼び出す必要あり
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

#include <thread>		// スレッド処理に必要

namespace {
	int nowInput = 0;
	int oldInput = 0;
	// 送るデータ構造体の宣言
	// 受信するデータ構造体もここに宣言
	// こいつを書き換えると送るデータも変わる
	SendDataWS2 realDataws2 = {};
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
	// リアルタイムサーバースレッド（仮）
	std::thread RealSendThread([&]() {
		realDataws2.Buffer = "REALKUSOZAKO";
		// ネットワーククラスの初期化
		// 引数には相手方のIPアドレスを入れる
		NetWorkWS2::Instance().Initialize("192.168.11.47");
		// リアルタイム通信の開始（サーバー）
		NetWorkWS2::Instance().RealTimeServer(realDataws2);
		});
	// スレッド開始
	RealSendThread.detach();
	//##############################################################

	//##############################################################
	// リアルタイムクライアントスレッド（仮）
	/*std::thread RealReciveThread([]() {
		// 受け取るデータを格納するバッファの作成
		dataws2.Buffer = "";
		// ネットワーククラスの初期化
		// 引数には相手方のIPアドレスを入れる
		NetWorkWS2::Instance().Initialize("192.168.11.55");
		// リアルタイム通信の開始（クライアント）
		NetWorkWS2::Instance().RealTimeClient(dataws2);
		});
	// スレッド開始
	RealReciveThread.detach();*/
	//##############################################################
}


TitleScene::~TitleScene()
{
}

void TitleScene::Update(const Peripheral& p)
{
	//##############################################################
	// 入力処理スレッド分け
	std::thread updateThread([&]() {
		(this->*_updater)(p);
		});
	updateThread.join();
	//##############################################################

	//##############################################################
	// ネットワーク通信呼び出し（仮）(サーバー)
	if (nowInput && !oldInput) {
		std::thread reciveThread([]() {
			SendDataWS2 dataws2 = {};
			dataws2.Buffer = "KUSOZAKO";
			// ネットワーククラスの初期化
			// 引数に相手方のIPアドレス
			NetWorkWS2::Instance().Initialize("192.168.11.47");
			// 非リアルタイム通信開始
			NetWorkWS2::Instance().SendServer(dataws2);
			});
		// スレッドの開始
		reciveThread.detach();
	}
	oldInput = nowInput;
	nowInput = CheckHitKey(KEY_INPUT_S);
	//##############################################################

	//##############################################################
	// ネットワーク通信呼び出し（仮）(クライアント)
	/*if (nowInput && !oldInput) {
		SendDataWS2 dataws2 = {};
		dataws2.Buffer = "";
		std::thread sendThread([&]() {
		// ネットワーククラスの初期化
			// 引数に相手方のIPアドレス
			NetWorkWS2::Instance().Initialize("192.168.11.55");
			// 非リアルタイム通信の開始
			NetWorkWS2::Instance().RecivedClient(dataws2);
			});
		// スレッドの開始
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
	// 描画処理スレッド分け
	std::thread drawThread([&]() {
		(this->*_drawer)();
		});
	drawThread.join();
	//########################################
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);
}
