#include <DxLib.h>
#include "../Stage.h"
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
	
	DataConfig(wCnt, tCnt, input, stageData);
	//// �t�H���g�T�C�Y�̕ύX��Y��Ȃ��悤�ɂ��Ă���

	Draw(wCnt, tCnt, stageData);

	DebugDraw(wCnt, tCnt, stageData);
}

void TargetType::Draw(const int& wCnt, const int& tCnt, const vec2_target stageData)
{
	SetFontSize(40);
	Vector2<int> drawPos;
	int strWidth, strHeight;
	std::string text;

	/// ���݁A�ݒ肵�Ă���I�̏�ԕ`��
	text = "�IID�̐ݒ�";
	GetDrawStringSize(&strWidth, &strHeight, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((Stage::GetInstance().GetScreenSize().x / 4) - (strWidth / 2), 0,
				text.c_str(), 0xffffff);

	/// �E�F�[�u���̐ݒ�
	text = "���݂̃E�F�[�u�� : " + std::to_string(wCnt);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, text.c_str(), strlen(text.c_str()));

	drawPos.x = Stage::GetInstance().GetScreenSize().x -
			   (Stage::GetInstance().GetScreenSize().x / 8) - (strWidth / 2);
	drawPos.y = strHeight / 2;
	
	DrawString(drawPos.x, drawPos.y, text.c_str(), 0xffffff);
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
