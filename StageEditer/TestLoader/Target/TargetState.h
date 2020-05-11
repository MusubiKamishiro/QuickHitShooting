#pragma once

#include "../Stage.h"
#include "../Input.h"

class TargetState
{
public:
	virtual void Update(int& wCnt, int& tCnt, 
						const unique_input& input, vec2_target& stageData) = 0;

protected:
	// �ݒ肷��I�̕ύX
	void ChangeTarget(int& tCnt, const int& tCntMax, const unique_input& input);

	// �ݒ肷��E�F�[�u�̕ύX
	void ChangeWave(int& wCnt, const int& wCntMax, const unique_input& input);

	void SetPosition(const int& wCnt, const int& tCnt,
					 const unique_input& input, vec2_target& stageData);

	void DebugDraw(const int& wCnt, const int& tCnt, vec2_target stageData);
private:
	
	// �X�e�[�W�f�[�^�̐ݒ�
	virtual void DataConfig(const int& wCnt, const int& tCnt,
							const unique_input& input, vec2_target& stageData) = 0;

	const int _boxSize = 50;
};

