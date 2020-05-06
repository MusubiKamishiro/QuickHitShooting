#pragma once
#include <DxLib.h>
#include <string>

struct SendData {
	std::string Buffer;
	bool result;
};

class NetWork
{
private:
	NetWork();
	NetWork(const NetWork&);
	void operator=(const NetWork&) {};

public:
	static NetWork& Instance() {
		static NetWork instance;
		return instance;
	}
	// �N���C�A���g���֐�
	void SetIP(int* ip);		// IP�A�h���X��ݒ肷��
	// ���ʊ֐�
	void Send(SendData* data);		// �f�[�^�𑗂�
	SendData Recive();		// �f�[�^���󂯎��
	

	~NetWork();

	SendData* dataBuffer = new SendData();		// �f�[�^�o�b�t�@
	IPDATA Ip = {};		// IP�A�h���X
	int NetHandle = 0;		// �l�b�g���[�N�n���h��
	int LostNetHandle = 0;		// �ؒf���ꂽ�l�b�g���[�N�̃n���h��
	int DataLength = 0;		// ��M�f�[�^�ʕۑ��ϐ�
	int Port = 2222;		// �ڑ��|�[�g�ԍ�
};

