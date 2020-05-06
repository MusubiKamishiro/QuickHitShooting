#pragma once
#include "Loader.h"

class StageLoader :
	public Loader
{
private:
	/// �X�e�[�W�f�[�^�ɕK�v�Ȃ��̂�����
public:
	StageLoader();
	~StageLoader();

	int Load(const std::string& path) override;

	void UnLoad(const std::string& path)override;
};

