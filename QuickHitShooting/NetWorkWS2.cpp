#include "NetWorkWS2.h"
#include <WS2tcpip.h>		// InetPton関数を使用するために必要
#include <cassert>		// アサートを使用するために必要
#include <iostream>
#include <thread>		// スレッドを使用するために必要

NetWorkWS2::~NetWorkWS2()
{
}

// サーバー関係の初期化
void NetWorkWS2::InitializeServer()
{
	// ポートなどの設定を行い、それをSOCKET構造体
	// に紐づけする
	// そのあとに接続待ちにする
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock0, (sockaddr*)&addr, sizeof(addr));
	listen(sock0, 5);
}

// クライアント関係の初期化
// 引数にIPアドレスを入れる
void NetWorkWS2::InitializeClient(const std::string& ip)
{
	// サーバー側のポートなどを設定する
	server.sin_family = AF_INET;
	server.sin_port = htons(Port);
	InetPton(server.sin_family,ip.c_str(), &server.sin_addr.S_un.S_addr);
	if (server.sin_addr.S_un.S_addr == 0xffffff) {
		assert(-1);
	}
}

void NetWorkWS2::SendServerCore(const char* data, size_t size)
{
	// 何度でも接続できるよう、whileでずっとループする
	while (true) {
		len = sizeof(client);
		// ここで接続してきたか確認
		sock = accept(sock0, (sockaddr*)&client, &len);

		// データを送る
		send(sock, data, size, 0);
		std::string returnData;
		returnData.resize(256);
		// 返信があるまで待機
		int n = recv(sock, (char*)returnData.c_str(), 256, 0);
		std::cout << returnData.c_str() << std::endl;
		// ソケットを閉じる
		closesocket(sock);
	}
}

void NetWorkWS2::RealTimeServerCore(const char* data, size_t size)
{
	len = sizeof(client);
	// クライアントから接続されたか確認する
	sock = accept(sock0, (sockaddr*)&client, &len);
	std::string returnData;
	returnData.resize(256);
	while (true) {
		// 構造体データを送信
		send(sock, data, size, 0);
		// 受信データの受け取る
		int n = recv(sock, (char*)returnData.c_str(), 256, 0);
		std::cout << returnData.c_str() << std::endl;
	}
	// ソケットを閉じる
	closesocket(sock);
}

// サーバ側処理
// 引数に送るデータを入れる
void NetWorkWS2::SendServer(SendDataWS2& data)
{
	// 構造体データを変換する
	const char* p = (const char*)&data;
	SendServerCore(p,sizeof(data));
}

// サーバー側処理（TargetData Ver）
// 引数に送るデータを入れる
void NetWorkWS2::SendServer(TargetData& data)
{
	// 構造体データを変換する
	const char* p = (const char*)&data;
	SendServerCore(p, sizeof(data));
}

void NetWorkWS2::SendServer(StageInfo& data)
{
	// 構造体データを変換する
	const char* p = (const char*)&data;
	SendServerCore(p, sizeof(data));
}

// クライアント側の処理
// 引数にデータを受け取るためのバッファを渡す。
void NetWorkWS2::RecivedClient(SendDataWS2& data)
{
	// サーバーに接続する関数
	connect(sock, (sockaddr*)&server, sizeof(server));
	// 接続出来たらデータを受け取る準備をする
	int n = recv(sock, (char*)&data, sizeof(data), 0);
	std::cout << n << data.Buffer << std::endl;
	// 受け取ったら文字列を返す
	send(sock, "Success", 7, 0);
	// ソケットを閉じる
	closesocket(sock);
}

// クライアント側の処理（TargetData Ver）
// 引数にデータを受け取るためのバッファを渡す。
void NetWorkWS2::RecivedClient(TargetData& data)
{
	// サーバーに接続する関数
	connect(sock, (sockaddr*)&server, sizeof(server));
	// データを受け取る
	int n = recv(sock, (char*)&data, sizeof(data), 0);
	// データを送る
	send(sock, "Success", 7, 0);
	// ソケットを閉じる
	closesocket(sock);
}

void NetWorkWS2::RecivedClient(StageInfo& data)
{
	// サーバーに接続する関数
	connect(sock, (sockaddr*)&server, sizeof(server));
	// データを受け取る
	int n = recv(sock, (char*)&data, sizeof(data), 0);
	// データを送る
	send(sock, "Success", 7, 0);
	// ソケットを閉じる
	closesocket(sock);
}

// リアルタイムサーバー処理
// 引数に送るデータを入れる
void NetWorkWS2::RealTimeServer(SendDataWS2& data)
{
	// 構造体データを変換
	const char* p = (const char*)&data;
	RealTimeServerCore(p,sizeof(data));
}

// リアルタイム通信サーバーの処理（TargetData Ver）
// 引数に送るデータを入れる
void NetWorkWS2::RealTimeServer(TargetData& data)
{
	// 構造体データを変換
	const char* p = (const char*)&data;
	RealTimeServerCore(p, sizeof(data));
}

void NetWorkWS2::RealTimeServer(StageInfo& data)
{
	// 構造体データを変換
	const char* p = (const char*)&data;
	RealTimeServerCore(p, sizeof(data));
}

// リアルタイム通信クライアントの処理
// 引数にデータを受け取るためのバッファを渡す。
void NetWorkWS2::RealTimeClient(SendDataWS2& data)
{
	// サーバーへ接続
	connect(sock, (sockaddr*)&server, sizeof(server));
	while (true) {
		// データの受信
		int n = recv(sock, (char*)&data, sizeof(data), 0);
		std::cout << data.Buffer << std::endl;
		// データの送信
		send(sock, "Success", 7, 0);
	}
	// ソケットを閉じる
	closesocket(sock);
}

// リアルタイム通信クライアント処理（TargetData Ver）
// 引数にデータを受け取るためのバッファを渡す。
void NetWorkWS2::RealTimeClient(TargetData& data)
{
	// サーバーへ接続
	connect(sock, (sockaddr*)&server, sizeof(server));
	while (true) {
		// データの受信
		int n = recv(sock, (char*)&data, sizeof(data), 0);
		// データの送信
		send(sock, "Success", 7, 0);
	}
	// ソケットを閉じる
	closesocket(sock);
}

void NetWorkWS2::RealTimeClient(StageInfo& data)
{
	// サーバーへ接続
	connect(sock, (sockaddr*)&server, sizeof(server));
	while (true) {
		// データの受信
		int n = recv(sock, (char*)&data, sizeof(data), 0);
		// データの送信
		send(sock, "Success", 7, 0);
	}
	// ソケットを閉じる
	closesocket(sock);
}

// WinSock2を終わるための処理
void NetWorkWS2::Terminate()
{
	WSACleanup();
}

// WinSock2を使用するための処理
void NetWorkWS2::Initialize(const std::string& ip)
{
	// WinSock2を初期化
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	// ソケットを初期化する
	sock0 = socket(AF_INET, SOCK_STREAM, 0);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	// サーバー、クライアントをそれぞれ初期化
	InitializeClient(ip);
	InitializeServer();
}