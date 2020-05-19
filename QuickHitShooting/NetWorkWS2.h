#pragma once
#include <WinSock2.h>
#include <string>

#pragma comment(lib,"ws2_32.lib")

struct SendDataWS2 {
	char* Buffer;
};

class NetWorkWS2
{
private:
	NetWorkWS2() {};
	NetWorkWS2(const NetWorkWS2&) {};
	void operator=(const NetWorkWS2&) {};

	void InitializeServer();
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
	static NetWorkWS2& Instance() {
		static NetWorkWS2 instance;
		return instance;
	}
	~NetWorkWS2();
	void Initialize(const std::string& ip);
	void SendServer(SendDataWS2 data);
	void RecivedClient(SendDataWS2 data);
	void Terminate();
};

