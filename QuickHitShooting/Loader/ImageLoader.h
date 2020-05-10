#pragma once
#include "Loader.h"
#include <map>


class ImageLoader : public Loader
{
private:
	///�摜�̃e�[�u���}�b�v
	///@param string �t�@�C���p�X
	///@param int �O���t�B�b�N�n���h��
	std::map<std::string, int> _table;

public:
	ImageLoader();
	~ImageLoader();

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

class ImageData : public Data
{
	friend ImageLoader;
private:
	int _handle;	// �n���h��

	///�_�~�[�֐�
	bool IsAvailable();

public:
	///�ǂݍ��񂾃n���h���̊l��
	///@return �O���t�B�b�N�n���h��
	int GetHandle()const;
};
