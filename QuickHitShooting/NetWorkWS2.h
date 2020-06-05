//############################################################
// WinSock2�Ńl�b�g���[�N�N���X�i�V���O���g���j
// WinSock2���g�p���ăf�[�^�̑���M���s���܂��B
// ��1�����DxLib�ƕ��p����ꍇ��DxLib�̃w�b�_�����O�ɌĂяo
// ���Ă��������B
// ��2����M�֐����Ăяo���Ƃ��͕K���X���b�h�ɕ����ČĂяo����
// ���������B
// �ŏI�X�V�F2020/5/29
// �S���F�R���\��
//############################################################

#pragma once
#include <WinSock2.h>		// WinSock2���g�p���邽�߂ɕK�v
#include <string>
#include "Loader/StageLoader.h"		// TargetData���g�����߂Ɏg�p

// WinSock2���g�p����ۂɕK�v��Lib
#pragma comment(lib,"ws2_32.lib")

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
	// �񃊃A���^�C���ʐM�֐����ʕ�
	void SendServerCore(const char* data,size_t size);
	// ���A���^�C���ʐM�֐����ʕ�
	void RealTimeServerCore(const char* data, size_t size);

	// ���L�ϐ�
	WSADATA wsaData = {};
	SOCKET sock = {};
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

	// �ڑ��o�������ǂ����̃`�F�b�N
	// �T�[�o�[��
	bool IsConnectionServer();
	// �N���C�A���g��
	bool IsConnectionClient();

	//####################################################
	// �T�[�o�[�������֐�
	// �N���C�A���g������ڑ�������΃f�[�^�𑗐M����
	// �ԐM������܂őҋ@���܂��B
	// �����ɑ���f�[�^������
	void SendServer(StageInfo& data);
	// �N���C�A���g�������֐�
	// �T�[�o�[�ɐڑ����ăf�[�^�������Ă���̂�ҋ@
	// �����Ă���������ł�����Ă����f�[�^�𑗂�Ԃ��B
	// �����Ƀf�[�^���󂯎�邽�߂̃o�b�t�@��n��
	void RecivedClient(StageInfo& data);
	//####################################################

	//####################################################
	// ���A���^�C���p�֐�
	// �T�[�o�[�������֐�
	// �񃊃A���^�C���ʐM�����A���^�C���ɉ��ǂ�������
	// �����Ƃ��Ă͂قړ���
	// �����ɑ���f�[�^������
	void RealTimeServer(StageInfo& data);
	// �N���C�A���g�������֐�
	// �񃊃A���^�C���ʐM�����A���^�C���ɉ��ǂ�������
	// �����͂قړ���
	// �����Ƀf�[�^���󂯎�邽�߂̃o�b�t�@��n��
	void RealTimeClient(StageInfo& data);
	//####################################################

	// �l�b�g���[�N�@�\�̌㏈��
	void Terminate();
};

