#pragma once
#include "Loader.h"

struct TargetData
{
	unsigned char type;
	unsigned int  dispTime;
	unsigned int appearTime;

	/// ���W������邩������Ȃ�
};

class StageLoader :
	public Loader
{
private:
	/// �X�e�[�W�f�[�^�ɕK�v�Ȃ��̂�����
	const char _waveEnd;
public:
	StageLoader();
	~StageLoader();

	int Load(const std::string& path) override;

	void UnLoad(const std::string& path)override;
};

