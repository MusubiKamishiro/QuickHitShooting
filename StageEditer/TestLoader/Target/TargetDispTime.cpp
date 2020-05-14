#include <DxLib.h>
#include "TargetDispTime.h"
#include "TargetAppearTime.h"

TargetDispTime::TargetDispTime()
{
}

TargetDispTime::~TargetDispTime()
{
}

void TargetDispTime::Update(int& wCnt, int& tCnt, 
							const unique_input& input, vec2_target& stageData)
{
	if (input->IsTrigger(KEY_INPUT_SPACE))
	{
		// �^�[�Q�b�g�ʒu�̏�����
		tCnt = 0;
		Stage::GetInstance().ChagneState(new TargetAppearTime());

		return;
	}

	ChangeTarget(tCnt, (int)stageData[wCnt].size(), input);
	ChangeWave(wCnt, (int)stageData.size(), input);
	SetPosition(wCnt, tCnt, input, stageData);
	DataConfig(wCnt, tCnt, input, stageData);

	DrawString(0, 0, "�I���o������܂ł̎��Ԑݒ�", 0xffffff);

	DebugDraw(wCnt, tCnt, stageData);
}

void TargetDispTime::DataConfig(const int& wCnt, const int& tCnt, 
								const unique_input& input, vec2_target& stageData)
{
	if (input->IsTrigger(KEY_INPUT_UP))
	{
		stageData[wCnt][tCnt].dispTime += 10;
	}
	else if (input->IsTrigger(KEY_INPUT_DOWN))
	{
		stageData[wCnt][tCnt].dispTime = (stageData[wCnt][tCnt].dispTime >= 10
									   ?  stageData[wCnt][tCnt].dispTime - 10
									   :  stageData[wCnt][tCnt].dispTime);
	}
	else {}
}