#include "NetWorkWS2.h"
#include <WS2tcpip.h>		// InetPton�֐����g�p���邽�߂ɕK�v
#include <cassert>		// �A�T�[�g���g�p���邽�߂ɕK�v
#include <iostream>
#include <thread>		// �X���b�h���g�p���邽�߂ɕK�v

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

void NetWorkWS2::SendServerCore(const char* data, size_t size)
{
	// ���x�ł��ڑ��ł���悤�Awhile�ł����ƃ��[�v����
	while (true) {
		len = sizeof(client);
		// �����Őڑ����Ă������m�F
		sock = accept(sock0, (sockaddr*)&client, &len);

		// �f�[�^�𑗂�
		send(sock, data, size, 0);
		std::string returnData;
		returnData.resize(256);
		// �ԐM������܂őҋ@
		int n = recv(sock, (char*)returnData.c_str(), 256, 0);
		std::cout << returnData.c_str() << std::endl;
		// �\�P�b�g�����
		closesocket(sock);
	}
}

void NetWorkWS2::RealTimeServerCore(const char* data, size_t size)
{
	len = sizeof(client);
	// �N���C�A���g����ڑ����ꂽ���m�F����
	sock = accept(sock0, (sockaddr*)&client, &len);
	std::string returnData;
	returnData.resize(256);
	while (true) {
		// �\���̃f�[�^�𑗐M
		send(sock, data, size, 0);
		// ��M�f�[�^�̎󂯎��
		int n = recv(sock, (char*)returnData.c_str(), 256, 0);
		std::cout << returnData.c_str() << std::endl;
	}
	// �\�P�b�g�����
	closesocket(sock);
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
		// �\���̃f�[�^��ϊ�����
		const char* p = (const char*)&data;
		// �f�[�^�𑗂�
		send(sock, data.Buffer.c_str(), data.Buffer.length() , 0);
		send(sock, (const char*)&data.data, sizeof(data.data) , 0);
		std::string returnData;
		returnData.resize(256);
		// �ԐM������܂őҋ@
		int n = recv(sock, (char*)returnData.c_str(), 256, 0);
		std::cout << returnData.c_str() << std::endl;
		// �\�P�b�g�����
		closesocket(sock);
	}
}

// �T�[�o�[�������iStageInfo Ver�j
// �����ɑ���f�[�^������
void NetWorkWS2::SendServer(StageInfo& data)
{
	// ���x�ł��ڑ��ł���悤�Awhile�ł����ƃ��[�v����
	while (true) {
		len = sizeof(client);
		// �����Őڑ����Ă������m�F
		sock = accept(sock0, (sockaddr*)&client, &len);
		// �f�[�^�𑗂�
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
		// �ԐM������܂őҋ@
		int n = recv(sock, (char*)returnData.c_str(), 256, 0);
		std::cout << returnData.c_str() << std::endl;
		// �\�P�b�g�����
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
	int n = recv(sock, (char*)data.Buffer.c_str(), data.Buffer.length(), 0);
	n = recv(sock, (char*)&data.data, sizeof(data.data), 0);
	std::cout << n << data.Buffer << std::endl;
	// �󂯎�����當�����Ԃ�
	send(sock, "Success", 7, 0);
	// �\�P�b�g�����
	closesocket(sock);
}

// �N���C�A���g���̏����iStageInfo Ver�j
// �����Ƀf�[�^���󂯎�邽�߂̃o�b�t�@��n���B
void NetWorkWS2::RecivedClient(StageInfo& data)
{
	// �T�[�o�[�ɐڑ�����֐�
	connect(sock, (sockaddr*)&server, sizeof(server));
	// �f�[�^���󂯎��
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
	// �f�[�^�𑗂�
	send(sock, "Success", 7, 0);
	// �\�P�b�g�����
	closesocket(sock);
}

// ���A���^�C���T�[�o�[����
// �����ɑ���f�[�^������
void NetWorkWS2::RealTimeServer(SendDataWS2& data)
{
	// �\���̃f�[�^��ϊ�
	const char* p = (const char*)&data;
	RealTimeServerCore(p,sizeof(data));
	
}

// ���A���^�C���ʐM�T�[�o�[�̏����iStageInfo Ver�j
// �����ɑ���f�[�^������
void NetWorkWS2::RealTimeServer(StageInfo& data)
{
	// �\���̃f�[�^��ϊ�
	const char* p = (const char*)&data;
	len = sizeof(client);
	// �N���C�A���g����ڑ����ꂽ���m�F����
	sock = accept(sock0, (sockaddr*)&client, &len);
	std::string returnData;
	returnData.resize(256);
	while (true) {
		// �f�[�^�̎󂯎��
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
		// ��M�f�[�^�̎󂯎��
		int n = recv(sock, (char*)returnData.c_str(), 256, 0);
		std::cout << returnData.c_str() << std::endl;
	}
	// �f�[�^�𑗂�
	send(sock, "Success", 7, 0);
	// �\�P�b�g�����
	closesocket(sock);
}

// ���A���^�C���ʐM�N���C�A���g�̏���
// �����Ƀf�[�^���󂯎�邽�߂̃o�b�t�@��n���B
void NetWorkWS2::RealTimeClient(SendDataWS2& data)
{
	// �T�[�o�[�֐ڑ�
	connect(sock, (sockaddr*)&server, sizeof(server));
	while (true) {
		// �f�[�^�̎�M
		int n = recv(sock, (char*)&data, sizeof(data), 0);
		std::cout << data.Buffer << std::endl;
		// �f�[�^�̑��M
		send(sock, "Success", 7, 0);
	}
	// �\�P�b�g�����
	closesocket(sock);
}

// ���A���^�C���ʐM�N���C�A���g�����iStageInfo Ver�j
// �����Ƀf�[�^���󂯎�邽�߂̃o�b�t�@��n���B
void NetWorkWS2::RealTimeClient(StageInfo& data)
{
	// �T�[�o�[�֐ڑ�
	connect(sock, (sockaddr*)&server, sizeof(server));
	while (true) {
		// �f�[�^���󂯎��
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
	// �\�P�b�g�����
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