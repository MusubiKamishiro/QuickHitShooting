#pragma once
#include <DxLib.h>
#include <string>

// �l�b�g���[�N�o�R�ő���f�[�^���܂Ƃ߂�\����
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

	SendData* dataBuffer = new SendData();		// �f�[�^�o�b�t�@
	IPDATA Ip = {};		// IP�A�h���X
	int NetHandle = 0;		// �l�b�g���[�N�n���h��
	int LostNetHandle = 0;		// �ؒf���ꂽ�l�b�g���[�N�̃n���h��
	int DataLength = 0;		// ��M�f�[�^�ʕۑ��ϐ�
	int Port = 2222;		// �ڑ��|�[�g�ԍ�

public:
	// �l�b�g���[�N�N���X�̃C���X�^���X��Ԃ�
	static NetWork& Instance() {
		static NetWork instance;
		return instance;
	}
	// �N���C�A���g���֐�
	void SetIP(int* ip);		// IP�A�h���X��ݒ肷��Aint�^�̔z�������
	// ���ʊ֐�
	void Send(SendData* data);		// �f�[�^�𑗂�
	SendData Recive();		// �f�[�^���󂯎��
	
	~NetWork();
};

