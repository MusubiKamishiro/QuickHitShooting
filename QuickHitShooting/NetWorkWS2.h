//############################################################
// WinSock2版ネットワーククラス（シングルトン）
// WinSock2を使用してデータの送受信を行います。
// ※1これをDxLibと併用する場合はDxLibのヘッダよりも前に呼び出
// してください。
// ※2送受信関数を呼び出すときは必ずスレッドに分けて呼び出して
// ください。
// 最終更新：2020/5/29
// 担当：山下圭介
//############################################################

#pragma once
#include <WinSock2.h>		// WinSock2を使用するために必要
#include <string>
#include "Loader/StageLoader.h"		// TargetDataを使うために使用

// WinSock2を使用する際に必要なLib
#pragma comment(lib,"ws2_32.lib")

// ネットワークで送受信するデータ構造体（Test）
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

	//####################################################
	// サーバー側処理関数
	// クライアント側から接続があればデータを送信して
	// 返信があるまで待機します。
	// 引数に送るデータを入れる
	void SendServer(SendDataWS2& data);
	void SendServer(TargetData& data);
	// クライアント側処理関数
	// サーバーに接続してデータが送られてくるのを待機
	// 送られてきたら引数でもらってきたデータを送り返す。
	// 引数にデータを受け取るためのバッファを渡す
	void RecivedClient(SendDataWS2& data);
	void RecivedClient(TargetData& data);
	//####################################################

	//####################################################
	// リアルタイム用関数
	// サーバー側処理関数
	// 非リアルタイム通信をリアルタイムに改良したもの
	// 処理としてはほぼ同じ
	// 引数に送るデータを入れる
	void RealTimeServer(SendDataWS2& data);
	void RealTimeServer(TargetData& data);
	// クライアント側処理関数
	// 非リアルタイム通信をリアルタイムに改良したもの
	// 処理はほぼ同じ
	// 引数にデータを受け取るためのバッファを渡す
	void RealTimeClient(SendDataWS2& data);
	void RealTimeClient(TargetData& data);
	//####################################################

	// ネットワーク機能の後処理
	void Terminate();
};

