#include <DxLib.h>
#include "TargetType.h"
#include "TargetDispTime.h"

TargetType::TargetType()
{
}

TargetType::~TargetType()
{
}

void TargetType::Update(int& wCnt, int& tCnt, 
						const unique_input& input, vec2_target& stageData)
{
	if (input->IsTrigger(KEY_INPUT_SPACE))
	{
		// �^�[�Q�b�g�ʒu�̏�����
		tCnt = 0;
		Stage::GetInstance().ChagneState(new TargetDispTime());
		
		return;
	}

	ChangeTarget(tCnt, (int)stageData[wCnt].size(), input);
	ChangeWave(wCnt, (int)stageData.size(), input);
	SetPosition(wCnt, tCnt, input, stageData);
	DataConfig(wCnt, tCnt, input, stageData);
	//// �t�H���g�T�C�Y�̕ύX��Y��Ȃ��悤�ɂ��Ă���
	DrawString(0, 0, "�IID�̐ݒ�", 0xffffff);

	DebugDraw(wCnt, tCnt, stageData);
}

void TargetType::DataConfig(const int& wCnt, const int& tCnt, 
							const unique_input& input, vec2_target& stageData)
{
	// �IID�̍ő�l�̎擾
	char idMax = static_cast<char>(TargetID::MAX);

	if (input->IsTrigger(KEY_INPUT_UP))
	{
		/// �IID�̉��Z
		stageData[wCnt][tCnt].type = (stageData[wCnt][tCnt].type + 1) % idMax;
	}
	else if (input->IsTrigger(KEY_INPUT_DOWN))
	{
		/// �IID�̌��Z
		stageData[wCnt][tCnt].type = ((stageData[wCnt][tCnt].type + idMax) - 1) % idMax;
	}
	else {}
}
