#pragma once
#include <DxLib.h>
#include <string>

struct SendData {
	std::string Buffer;
	bool result;
};

class NetWork
{
private:
	NetWork();
	NetWork(const NetWork&);
	void operator=(const NetWork&) {};

public:
	static NetWork& Instance() {
		static NetWork instance;
		return instance;
	}
	// クライアント側関数
	void SetIP(int* ip);		// IPアドレスを設定する
	// 共通関数
	void Send(SendData* data);		// データを送る
	SendData Recive();		// データを受け取る
	

	~NetWork();

	SendData* dataBuffer = new SendData();		// データバッファ
	IPDATA Ip = {};		// IPアドレス
	int NetHandle = 0;		// ネットワークハンドル
	int LostNetHandle = 0;		// 切断されたネットワークのハンドル
	int DataLength = 0;		// 受信データ量保存変数
	int Port = 2222;		// 接続ポート番号
};

