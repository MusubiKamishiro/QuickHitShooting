#include "TargetState.h"

void TargetState::ChangeTarget(int& tCnt, const int& tCntMax, const unique_input& input)
{
	if (input->IsTrigger(KEY_INPUT_LEFT))
	{
		tCnt = ((tCnt + tCntMax) - 1) % tCntMax;
	}
	else if (input->IsTrigger(KEY_INPUT_RIGHT))
	{
		tCnt = (tCnt + 1) % tCntMax;
	}
	else {}
}

void TargetState::ChangeWave(int& wCnt, const int& wCntMax, const unique_input& input)
{
	if ((input->IsPressed(KEY_INPUT_RSHIFT) || input->IsPressed(KEY_INPUT_LSHIFT)) &&
		 input->IsTrigger(KEY_INPUT_DOWN))
	{
		wCnt = ((wCnt + wCntMax) - 1) % wCntMax;
	}
	else if ((input->IsPressed(KEY_INPUT_RSHIFT) || input->IsPressed(KEY_INPUT_LSHIFT)) &&
			  input->IsTrigger(KEY_INPUT_UP))
	{
		wCnt = (wCnt + 1) % wCntMax;
	}
	else {}
}

void TargetState::SetPosition(const int& wCnt, const int& tCnt, 
							  const unique_input& input, vec2_target& stageData)
{
	if (input->IsMouseTrigger(MOUSE_INPUT_LEFT))
	{
		/// �I�̍��W���X�V����
		GetMousePoint(&stageData[wCnt][tCnt].posX, &stageData[wCnt][tCnt].posY);
	}
}

void TargetState::DebugDraw(const int& wCnt, const int& tCnt, vec2_target stageData)
{
	int strWidth, strHeight;
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "���݂̃E�F�[�u : ", strlen("���݂̃E�F�[�u : "));
	DrawString(1000, 0, "���݂̃E�F�[�u : ", 0xffffff);
	DrawFormatString(1000 + strWidth, 0, 0xffffff, "%d", wCnt);

	GetDrawStringSize(&strWidth, &strHeight, nullptr, "���݂̃^�[�Q�b�g : ", strlen("���݂̃^�[�Q�b�g : "));
	DrawString(1000, 40, "���݂̃^�[�Q�b�g : ", 0xffffff);
	DrawFormatString(1000 + strWidth, 40, 0xffffff, "%d", tCnt);

	GetDrawStringSize(&strWidth, &strHeight, nullptr, "�^�[�Q�b�g�^�C�v :  ", strlen("�^�[�Q�b�g�^�C�v : "));
	DrawString(1000, 40 * 2, "�^�[�Q�b�g�^�C�v : ", 0xffffff);
	DrawFormatString(1000 + strWidth, 40 * 2, 0xffffff, "%d", stageData[wCnt][tCnt].type);

	GetDrawStringSize(&strWidth, &strHeight, nullptr, "�o�����鎞�� : ", strlen("�o�����鎞�� : "));
	DrawString(1000, 40 * 3, "�o�����鎞�� : ", 0xffffff);
	DrawFormatString(1000 + strWidth, 40 * 3, 0xffffff, "%d", stageData[wCnt][tCnt].dispTime);

	GetDrawStringSize(&strWidth, &strHeight, nullptr, "������܂ł̎��� : ", strlen("������܂ł̎��� : "));
	DrawString(1000, 40 * 4, "������܂ł̎��� : ", 0xffffff);
	DrawFormatString(1000 + strWidth, 40 * 4, 0xffffff, "%d", stageData[wCnt][tCnt].appearTime);

	int posX, posY;
	GetMousePoint(&posX, &posY);
	DrawBox(posX - (_boxSize / 2), posY - (_boxSize / 2),
			posX + (_boxSize / 2), posY + (_boxSize / 2),
			0x7fffd4, true);

	posX = stageData[wCnt][tCnt].posX;
	posY = stageData[wCnt][tCnt].posY;
	DrawBox(posX - (_boxSize / 2), posY - (_boxSize / 2),
			posX + (_boxSize / 2), posY + (_boxSize / 2),
			0xff4500, true);
}
