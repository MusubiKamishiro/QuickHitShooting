#include <DxLib.h>
#include "TargetAppearTime.h"
#include "TargetType.h"

TargetAppearTime::TargetAppearTime()
{
}

TargetAppearTime::~TargetAppearTime()
{
}

void TargetAppearTime::Update(int& wCnt, int& tCnt, 
							  const unique_input& input, vec2_target& stageData)
{
	if (input->IsTrigger(KEY_INPUT_SPACE))
	{
		// �^�[�Q�b�g�ʒu�̏�����
		tCnt = 0;
		Stage::GetInstance().ChagneState(new TargetType());
		return;
	}

	ChangeTarget(tCnt, (int)stageData[wCnt].size(), input);
	ChangeWave(wCnt, (int)stageData.size(), input);
	SetPosition(wCnt, tCnt, input, stageData);
	DataConfig(wCnt, tCnt, input, stageData);

	DrawString(0, 0, "�I��������܂ł̎��Ԑݒ�", 0xffffff);

	DebugDraw(wCnt, tCnt, stageData);
}

void TargetAppearTime::DataConfig(const int& wCnt, const int& tCnt, const unique_input& input, vec2_target& stageData)
{
	if (input->IsTrigger(KEY_INPUT_UP))
	{
		stageData[wCnt][tCnt].appearTime += 10;
	}
	else if (input->IsTrigger(KEY_INPUT_DOWN))
	{
		stageData[wCnt][tCnt].appearTime = (stageData[wCnt][tCnt].appearTime >= 10
										 ?  stageData[wCnt][tCnt].appearTime - 10
										 :  stageData[wCnt][tCnt].appearTime);
	}
	else {}
}
