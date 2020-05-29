#pragma once
#include "Loader.h"
#include "../Geometry.h"
#include <map>
#include <vector>
#include <array>

// �I���
struct TargetData
{
	unsigned char type;			// ID
	unsigned int  dispTime;		// �\������
	unsigned int  appearTime;	// �\�����Ă��������܂ł̎���
	Vector2<int>  pos;			// ���W
};
// �I���̉ϒ��z��
using vec_target = std::vector<TargetData>;

struct StageInfo
{
	std::array<int, 3> scores;
	std::array<std::string, 3> names;
	std::vector<vec_target> targetData;

	/// �X�e�[�W�l�[���擾�p
	std::string stageName;
};

class StageLoader : public Loader
{
private:
	///�X�e�[�W�̃e�[�u���}�b�v
	///@param string �t�@�C���p�X
	///@param TargetData �T�E���h�n���h��
	std::map<std::string, StageInfo> _table;
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
	void UnLoad(const std::string& path) override;
};

class StageData : public Data
{
	friend StageLoader;
private:
	StageInfo _stageData;

	///�_�~�[�֐�
	bool IsAvailable();

public:
	StageInfo GetStageData() const;
};