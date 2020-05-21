#pragma once
#include "Loader.h"
#include <map>


class SoundLoader : public Loader
{
private:
	///�T�E���h�̃e�[�u���}�b�v
	///@param string �t�@�C���p�X
	///@param int �T�E���h�n���h��
	std::map<std::string, int> _table;

public:
	SoundLoader();
	~SoundLoader();

	///���[�h
	///@param path �t�@�C���̃p�X
	///@param data �f�[�^�I�u�W�F�N�g�ւ̎Q��
	///@retval true ����
	///@retval false ���s
	bool Load(const std::string& path, Data& data)override final;

	/// �A�����[�h
	///@param path �t�@�C���̃p�X
	void UnLoad(const std::string& path)override final;
};

class SoundData : public Data
{
	friend SoundLoader;
private:
	int _handle;	// �n���h��

	///�_�~�[�֐�
	bool IsAvailable();

public:
	///�ǂݍ��񂾃n���h���̊l��
	///@return �T�E���h�n���h��
	int GetHandle()const;
};