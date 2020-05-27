//############################################################
// WinSock2�Ńl�b�g���[�N�N���X
// WinSock2���g�p���ăf�[�^�̑���M���s���܂��B
// �S���F�R���\��
//############################################################

#pragma once
#include <WinSock2.h>
#include <string>
#include "Loader/StageLoader.h"

// WinSock2���g�p����ۂɕK�v��Lib
#pragma comment(lib,"ws2_32.lib")

// �l�b�g���[�N�ő���M����f�[�^�\����
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

	// �T�[�o���@�\�̏�����
	void InitializeServer();
	// �N���C�A���g���@�\�̏�����
	void InitializeClient(const std::string& ip);

	// ���L�ϐ�
	WSADATA wsaData = {};
	SOCKET sock = {};
	SendDataWS2 dataBuffer = {};
	int Port = 2222;

	// �T�[�o�[���ϐ�
	sockaddr_in addr = {};
	sockaddr_in client = {};
	SOCKET sock0 = {};
	int len = 0;

	// �N���C�A���g���ϐ�
	sockaddr_in server = {};

public:
	//�l�b�g���[�N�N���X�̃C���X�^���X
	static NetWorkWS2& Instance() {
		static NetWorkWS2 instance;
		return instance;
	}
	~NetWorkWS2();
	// �T�[�o�A�N���C�A���g�̏���������
	// �����ɒʐM�����������IP�A�h���X�����Ă�������
	void Initialize(const std::string& ip);
	// �T�[�o�[�������֐�
	// �N���C�A���g������ڑ�������΃f�[�^�𑗐M����
	// �ԐM������܂őҋ@���܂��B
	// �����Ɏ󂯎��f�[�^�̃o�b�t�@��n���܂��B
	void SendServer(SendDataWS2& data);
	void SendServer(TargetData& data);
	// �N���C�A���g�������֐�
	// �T�[�o�[�ɐڑ����ăf�[�^�������Ă���̂�ҋ@
	// �����Ă���������ł�����Ă����f�[�^�𑗂�Ԃ��B
	// �����ɑ���f�[�^��n���܂��B
	void RecivedClient(SendDataWS2& data);
	void RecivedClient(TargetData& data);

	// ���A���^�C���p�֐�
	// �T�[�o�[�������֐�
	void RealTimeServer(TargetData& data);
	// �N���C�A���g�������֐�
	void RealTimeClient(TargetData& data);

	// �l�b�g���[�N�@�\�̌㏈��
	void Terminate();
};

