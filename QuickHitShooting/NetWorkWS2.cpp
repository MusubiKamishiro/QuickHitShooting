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

void NetWorkWS2::SendServer(SendDataWS2 data)
{
	while (true) {
		len = sizeof(client);
		sock = accept(sock0, (sockaddr*)&client, &len);
		send(sock, (const char*)&data, sizeof(SendDataWS2), 0);
		SendDataWS2 returnData = {};
		int n = recv(sock, (char*)&returnData, sizeof(SendDataWS2), 0);
		std::cout << returnData.Buffer << std::endl;
		closesocket(sock);
	}
}

void NetWorkWS2::RecivedClient(SendDataWS2 data)
{
	connect(sock, (sockaddr*)&server, sizeof(server));
	int n = recv(sock, (char*)&data, sizeof(SendDataWS2),0);
	std::cout << n << data.Buffer << std::endl;
	SendDataWS2 sendData = {};
	sendData.Buffer = "Success";
	send(sock, (const char*)&sendData, 7, 0);
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