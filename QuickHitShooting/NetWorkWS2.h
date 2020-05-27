//############################################################
// WinSock2版ネットワーククラス
// WinSock2を使用してデータの送受信を行います。
// 担当：山下圭介
//############################################################

#pragma once
#include <WinSock2.h>
#include <string>
#include "Loader/StageLoader.h"

// WinSock2を使用する際に必要なLib
#pragma comment(lib,"ws2_32.lib")

// ネットワークで送受信するデータ構造体
struct SendDataWS2 {
	std::string Buffer;
	int data;
};

class NetWorkWS2
{
private:
	NetWorkWS2() {};
	NetWorkWS2(const NetWorkWS2&) {};
	void operator=(const NetWorkWS2&) {};

	// サーバ側機能の初期化
	void InitializeServer();
	// クライアント側機能の初期化
	void InitializeClient(const std::string& ip);

	// 共有変数
	WSADATA wsaData = {};
	SOCKET sock = {};
	SendDataWS2 dataBuffer = {};
	int Port = 2222;

	// サーバー側変数
	sockaddr_in addr = {};
	sockaddr_in client = {};
	SOCKET sock0 = {};
	int len = 0;

	// クライアント側変数
	sockaddr_in server = {};

public:
	//ネットワーククラスのインスタンス
	static NetWorkWS2& Instance() {
		static NetWorkWS2 instance;
		return instance;
	}
	~NetWorkWS2();
	// サーバ、クライアントの初期化処理
	// 引数に通信したい相手のIPアドレスを入れてください
	void Initialize(const std::string& ip);
	// サーバー側処理関数
	// クライアント側から接続があればデータを送信して
	// 返信があるまで待機します。
	// 引数に受け取るデータのバッファを渡します。
	void SendServer(SendDataWS2& data);
	void SendServer(TargetData& data);
	// クライアント側処理関数
	// サーバーに接続してデータが送られてくるのを待機
	// 送られてきたら引数でもらってきたデータを送り返す。
	// 引数に送るデータを渡します。
	void RecivedClient(SendDataWS2& data);
	void RecivedClient(TargetData& data);

	// リアルタイム用関数
	// サーバー側処理関数
	void RealTimeServer(TargetData& data);
	// クライアント側処理関数
	void RealTimeClient(TargetData& data);

	// ネットワーク機能の後処理
	void Terminate();
};

