#include "NetWorkWS2.h"
#include <WS2tcpip.h>
#include <DxLib.h>
#include <cassert>
#include <iostream>

NetWorkWS2::~NetWorkWS2()
{
}

void NetWorkWS2::InitializeServer()
{
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock0, (sockaddr*)&addr, sizeof(addr));
	listen(sock0, 5);
}

void NetWorkWS2::InitializeClient(const std::string& ip)
{
	server.sin_family = AF_INET;
	server.sin_port = htons(Port);
	InetPton(server.sin_family,ip.c_str(), &server.sin_addr.S_un.S_addr);
	if (server.sin_addr.S_un.S_addr == 0xffffff) {
		assert(-1);
	}
}

void NetWorkWS2::SendServer(SendDataWS2& data)
{
	while (true) {
		len = sizeof(client);
		sock = accept(sock0, (sockaddr*)&client, &len);
		const char* p = (const char*)&data;
		send(sock, p, sizeof(data), 0);
		std::string returnData = "";
		int n = recv(sock, (char*)returnData.c_str(), 256, 0);
		std::cout << returnData.c_str() << std::endl;
		closesocket(sock);
	}
}

void NetWorkWS2::SendServer(TargetData& data)
{
	while (true) {
		len = sizeof(client);
		sock = accept(sock0, (sockaddr*)&client, &len);
		const char* p = (const char*)&data;
		send(sock, p, sizeof(data), 0);
		std::string returnData = "";
		int n = recv(sock, (char*)returnData.c_str(), 256, 0);
		std::cout << returnData.c_str() << std::endl;
		closesocket(sock);
	}
}

void NetWorkWS2::RecivedClient(SendDataWS2& data)
{
	connect(sock, (sockaddr*)&server, sizeof(server));
	int n = recv(sock, (char*)&data, sizeof(data),0);
	std::cout << n << data.Buffer << std::endl;
	std::string sendData = "Success";
	send(sock, sendData.c_str(), sendData.length(), 0);
	closesocket(sock);
}

void NetWorkWS2::RecivedClient(TargetData& data)
{
	connect(sock, (sockaddr*)&server, sizeof(server));
	int n = recv(sock, (char*)&data, sizeof(data), 0);
	std::string sendData = "Success";
	send(sock, sendData.c_str(), sendData.length(), 0);
	closesocket(sock);
}

void NetWorkWS2::Terminate()
{
	WSACleanup();
}

void NetWorkWS2::Initialize(const std::string& ip)
{
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	sock0 = socket(AF_INET, SOCK_STREAM, 0);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	InitializeClient(ip);
	InitializeServer();
}