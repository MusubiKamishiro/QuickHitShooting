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
	//Initialize();
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock0, (sockaddr*)&addr, sizeof(addr));
	listen(sock0, 5);
}

void NetWorkWS2::InitializeClient(const std::string& ip)
{
	//Initialize();
	server.sin_family = AF_INET;
	server.sin_port = htons(Port);
	InetPton(server.sin_family,ip.c_str(), &server.sin_addr.S_un.S_addr);
	if (server.sin_addr.S_un.S_addr == 0xffffff) {
		assert(-1);
	}
	dataBuffer.Buffer.resize(256);
}

void NetWorkWS2::ReciveServer(SendDataWS2& data)
{
	while (true) {
		len = sizeof(client);
		sock = accept(sock0, (sockaddr*)&client, &len);
		send(sock, "HELLO", 5, 0);
		SendDataWS2 returnData;
		recv(sock, (char*)returnData.Buffer.c_str(), sizeof(returnData.Buffer), 0);
		std::cout << returnData.Buffer << std::endl;
		closesocket(sock);
	}
}

void NetWorkWS2::SendClient(SendDataWS2& data)
{
	connect(sock, (sockaddr*)&server, sizeof(server));
	int n = recv(sock, (char*)data.Buffer.c_str(), sizeof(data.Buffer),0);
	std::cout << n << data.Buffer << std::endl;
	SendDataWS2 reciveData;
	len = sizeof(client);
	sock = accept(sock0, (sockaddr*)&client, &len);
	send(sock, "Success", 7, 0);
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