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

// 非リアルタイム通信処理　共通部
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

// リアルタイム通信処理　共通部
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

// サーバー側処理（StageInfo Ver）
// 引数に送るデータを入れる
void NetWorkWS2::SendServer(StageInfo& data)
{
	// 何度でも接続できるよう、whileでずっとループする
	while (true) {
		len = sizeof(client);
		// ここで接続してきたか確認
		sock = accept(sock0, (sockaddr*)&client, &len);
		// データを送る
		send(sock, (const char*)&data.names, sizeof(data.names[0]) * data.names.size(), 0);
		send(sock, (const char*)&data.scores, sizeof(data.scores[0]) * data.scores.size(), 0);
		auto size = data.targetData.size();
		send(sock, (const char*)&size, sizeof(size_t), 0);
		for (auto& target : data.targetData) {
			size = target.size();
			send(sock, (const char*)&size, sizeof(size_t), 0);
			for (auto i = 0; i < size; i++) {
				send(sock, (const char*)&target[i], sizeof(target[i]), 0);
			}
		}
		
		std::string returnData;
		returnData.resize(256);
		// 返信があるまで待機
		int n = recv(sock, (char*)returnData.c_str(), 256, 0);
		std::cout << returnData.c_str() << std::endl;
		// ソケットを閉じる
		closesocket(sock);
	}
}

// クライアント側の処理（StageInfo Ver）
// 引数にデータを受け取るためのバッファを渡す。
void NetWorkWS2::RecivedClient(StageInfo& data)
{
	// サーバーに接続する関数
	connect(sock, (sockaddr*)&server, sizeof(server));
	// データを受け取る
	int n = recv(sock, (char*)&data.names, sizeof(data.names[0]) * data.names.size(), 0);
	n = recv(sock, (char*)&data.scores, sizeof(data.scores[0]) * data.scores.size(), 0);
	size_t size = 0;
	n = recv(sock, (char*)&size, sizeof(size_t), 0);
	data.targetData.resize(size);
	for (auto& target : data.targetData) {
		n = recv(sock, (char*)size, sizeof(size_t), 0);
		for (auto i = 0; i < size; i++) {
			target.push_back(TargetData());
			n = recv(sock, (char*)&target[i], sizeof(TargetData), 0);
		}
	}
	// データを送る
	send(sock, "Success", 7, 0);
	// ソケットを閉じる
	closesocket(sock);
}

// リアルタイム通信サーバーの処理（StageInfo Ver）
// 引数に送るデータを入れる
void NetWorkWS2::RealTimeServer(StageInfo& data)
{
	// 構造体データを変換
	const char* p = (const char*)&data;
	len = sizeof(client);
	// クライアントから接続されたか確認する
	sock = accept(sock0, (sockaddr*)&client, &len);
	std::string returnData;
	returnData.resize(256);
	while (true) {
		// データの受け取り
		send(sock, (const char*)&data.names, sizeof(data.names[0]) * data.names.size(), 0);
		send(sock, (const char*)&data.scores, sizeof(data.scores[0]) * data.scores.size(), 0);
		auto size = data.targetData.size();
		send(sock, (const char*)&size, sizeof(size_t), 0);
		for (auto& target : data.targetData) {
			size = target.size();
			send(sock, (const char*)&size, sizeof(size_t), 0);
			for (auto i = 0; i < size; i++) {
				send(sock, (const char*)&target[i], sizeof(target[i]), 0);
			}
		}
		// 受信データの受け取る
		int n = recv(sock, (char*)returnData.c_str(), 256, 0);
		std::cout << returnData.c_str() << std::endl;
	}
	// データを送る
	send(sock, "Success", 7, 0);
	// ソケットを閉じる
	closesocket(sock);
}

// リアルタイム通信クライアント処理（StageInfo Ver）
// 引数にデータを受け取るためのバッファを渡す。
void NetWorkWS2::RealTimeClient(StageInfo& data)
{
	// サーバーへ接続
	connect(sock, (sockaddr*)&server, sizeof(server));
	while (true) {
		// データを受け取る
		int n = recv(sock, (char*)&data.names, sizeof(data.names[0]) * data.names.size(), 0);
		n = recv(sock, (char*)&data.scores, sizeof(data.scores[0]) * data.scores.size(), 0);
		size_t size = 0;
		n = recv(sock, (char*)&size, sizeof(size_t), 0);
		data.targetData.resize(size);
		for (auto& target : data.targetData) {
			n = recv(sock, (char*)size, sizeof(size_t), 0);
			for (auto i = 0; i < size; i++) {
				target.push_back(TargetData());
				n = recv(sock, (char*)&target[i], sizeof(TargetData), 0);
			}
		}
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

// サーバー側接続できるか確認用関数
bool NetWorkWS2::IsConnectionServer()
{
	len = sizeof(client);
	sock = accept(sock0, (sockaddr*)&client, &len);
	if (sock != INVALID_SOCKET) {
		closesocket(sock);
		return true;
	}
	closesocket(sock);
	return false;
}

// クライアント側接続できるか確認用関数
bool NetWorkWS2::IsConnectionClient()
{
	if (connect(sock, (sockaddr*)&server, sizeof(server)) == 0) {
		closesocket(sock);
		return true;
	}
	closesocket(sock);
	return false;
}
