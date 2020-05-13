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
	void Recive(SendData& data);		// �f�[�^���󂯎��
	
	~NetWork();
};

