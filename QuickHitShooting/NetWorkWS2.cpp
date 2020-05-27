#include "NetWorkWS2.h"
#include <WS2tcpip.h>
#include <cassert>
#include <iostream>
#include <thread>

NetWorkWS2::~NetWorkWS2()
{
}

// �T�[�o�[�֌W�̏�����
void NetWorkWS2::InitializeServer()
{
	// �|�[�g�Ȃǂ̐ݒ���s���A�����SOCKET�\����
	// �ɕR�Â�����
	// ���̂��Ƃɐڑ��҂��ɂ���
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock0, (sockaddr*)&addr, sizeof(addr));
	listen(sock0, 5);
}

// �N���C�A���g�֌W�̏�����
// ������IP�A�h���X������
void NetWorkWS2::InitializeClient(const std::string& ip)
{
	// �T�[�o�[���̃|�[�g�Ȃǂ�ݒ肷��
	server.sin_family = AF_INET;
	server.sin_port = htons(Port);
	InetPton(server.sin_family,ip.c_str(), &server.sin_addr.S_un.S_addr);
	if (server.sin_addr.S_un.S_addr == 0xffffff) {
		assert(-1);
	}
}

// �T�[�o������
// �����ɑ���f�[�^������
void NetWorkWS2::SendServer(SendDataWS2& data)
{
	// ���x�ł��ڑ��ł���悤�Awhile�ł����ƃ��[�v����
	while (true) {
		len = sizeof(client);
		// �����Őڑ����Ă������m�F
		sock = accept(sock0, (sockaddr*)&client, &len);
		const char* p = (const char*)&data;
		// �f�[�^�𑗂�
		send(sock, p, sizeof(data), 0);
		std::string returnData;
		returnData.resize(256);
		// �ԐM������܂őҋ@
		int n = recv(sock, (char*)returnData.c_str(), 256, 0);
		std::cout << returnData.c_str() << std::endl;
		closesocket(sock);
	}
}

// �T�[�o�[�������iTargetData Ver�j
// �����ɑ���f�[�^������
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

// �N���C�A���g���̏���
// �����Ƀf�[�^���󂯎�邽�߂̃o�b�t�@��n���B
void NetWorkWS2::RecivedClient(SendDataWS2& data)
{
	// �T�[�o�[�ɐڑ�����֐�
	connect(sock, (sockaddr*)&server, sizeof(server));
	// �ڑ��o������f�[�^���󂯎�鏀��������
	int n = recv(sock, (char*)&data, sizeof(data),0);
	std::cout << n << data.Buffer << std::endl;
	// �󂯎�����當�����Ԃ�
	send(sock, "Success", 7, 0);
	closesocket(sock);
}

// �N���C�A���g���̏����iTargetData Ver�j
// �����Ƀf�[�^���󂯎�邽�߂̃o�b�t�@��n���B
void NetWorkWS2::RecivedClient(TargetData& data)
{
	connect(sock, (sockaddr*)&server, sizeof(server));
	int n = recv(sock, (char*)&data, sizeof(data), 0);
	send(sock, "Success", 7, 0);
	closesocket(sock);
}

void NetWorkWS2::RealTimeServer(SendDataWS2& data)
{
	len = sizeof(client);
	sock = accept(sock0, (sockaddr*)&client, &len);
	const char* p = (const char*)&data;
	while (true) {
		send(sock, p, sizeof(data), 0);
		std::string returnData;
		returnData.resize(256);
		int n = recv(sock, (char*)returnData.c_str(), 256, 0);
		std::cout << returnData.c_str() << std::endl;
	}
	closesocket(sock);
}

// ���A���^�C���ʐM�T�[�o�[�̏���
void NetWorkWS2::RealTimeServer(TargetData& data)
{
	len = sizeof(client);
	sock = accept(sock0, (sockaddr*)&client, &len);
	const char* p = (const char*)&data;
	while (true) {
		send(sock, p, sizeof(data), 0);
		std::string returnData = "";
		int n = recv(sock, (char*)returnData.c_str(), 256, 0);
		std::cout << returnData.c_str() << std::endl;
	}
	closesocket(sock);
}

void NetWorkWS2::RealTimeClient(SendDataWS2& data)
{
	connect(sock, (sockaddr*)&server, sizeof(server));
	while (true) {
		int n = recv(sock, (char*)&data, sizeof(data), 0);
		send(sock, "Success", 7, 0);
	}
	closesocket(sock);
}

// ���A���^�C���ʐM�N���C�A���g����
void NetWorkWS2::RealTimeClient(TargetData& data)
{
	connect(sock, (sockaddr*)&server, sizeof(server));
	while (true) {
		int n = recv(sock, (char*)&data, sizeof(data), 0);
		send(sock, "Success", 7, 0);
	}
	closesocket(sock);
}

// WinSock2���I��邽�߂̏���
void NetWorkWS2::Terminate()
{
	WSACleanup();
}

// WinSock2���g�p���邽�߂̏���
void NetWorkWS2::Initialize(const std::string& ip)
{
	// WinSock2��������
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	// �\�P�b�g������������
	sock0 = socket(AF_INET, SOCK_STREAM, 0);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	// �T�[�o�[�A�N���C�A���g�����ꂼ�ꏉ����
	InitializeClient(ip);
	InitializeServer();
}