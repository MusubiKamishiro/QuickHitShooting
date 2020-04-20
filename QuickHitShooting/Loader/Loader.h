#pragma once
#include <string>

///���[�h���N���X
class Loader
{
public:
	Loader();
	virtual ~Loader();

	///���[�h
	///@param path �t�@�C���̃p�X
	///@param data �f�[�^�I�u�W�F�N�g�ւ̎Q��
	///@retval true ����
	///@retval false ���s
	virtual int Load(const std::string& path) = 0;

	///�f�[�^�̃A�����[�h
	///@attention	����Ŏw�肳�ꂽ�p�X�ɑΉ�����f�[�^�̓������ォ�疳���Ȃ�̂�
	///				�������g�p����Ƃ��̓X�g���[�W�A�N�Z�X�����������
	virtual void UnLoad(const std::string& path) = 0;
};

