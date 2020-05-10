#pragma once
#include "Loader.h"
#include <map>

struct TargetData
{
	unsigned char type;
	unsigned int  dispTime;
	unsigned int appearTime;

	/// ���W������邩������Ȃ�
};

class StageLoader : public Loader
{
private:
	///�X�e�[�W�̃e�[�u���}�b�v
	///@param string �t�@�C���p�X
	///@param TargetData �T�E���h�n���h��
	std::map<std::string, TargetData> _table;
	/// �X�e�[�W�f�[�^�ɕK�v�Ȃ��̂�����
	const char _waveEnd;
public:
	StageLoader();
	~StageLoader();

	///���[�h
	///@param path �t�@�C���̃p�X
	///@param data �f�[�^�I�u�W�F�N�g�ւ̎Q��
	///@retval true ����
	///@retval false ���s
	bool Load(const std::string& path, Data& data) override;

	/// �A�����[�h
	///@param path �t�@�C���̃p�X
	void UnLoad(const std::string& path)override;
};

class StageData : public Data
{
	friend StageLoader;
private:
	TargetData _targetData;

	///�_�~�[�֐�
	bool IsAvailable();

public:
	TargetData GetTargetData()const;
};