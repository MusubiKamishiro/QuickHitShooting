#include "TargetState.h"

void TargetState::ChangeTarget(int& tCnt, const int& tCntMax, const unique_input& input)
{
	if (!input->IsPressed(KEY_INPUT_RSHIFT) && !input->IsPressed(KEY_INPUT_LSHIFT) &&
		(input->IsTrigger(KEY_INPUT_UP)		||  input->IsTrigger(KEY_INPUT_S)))
	{
		/// �^�[�Q�b�g�J�E���g�����Z����
		tCnt = ((tCnt + tCntMax) - 1) % tCntMax;
	}
	else if (!input->IsPressed(KEY_INPUT_RSHIFT) && !input->IsPressed(KEY_INPUT_LSHIFT) &&
			 (input->IsTrigger(KEY_INPUT_DOWN)   ||  input->IsTrigger(KEY_INPUT_W)))
	{
		/// �^�[�Q�b�g�J�E���g�����Z����
		tCnt = (tCnt + 1) % tCntMax;
	}
	else {}
}

void TargetState::ChangeWave(int& wCnt, const int& wCntMax, const unique_input& input)
{
	if ((input->IsPressed(KEY_INPUT_RSHIFT) || input->IsPressed(KEY_INPUT_LSHIFT)) &&
		(input->IsTrigger(KEY_INPUT_UP)		|| input->IsTrigger(KEY_INPUT_S)))
	{
		/// �E�F�[�u�J�E���g�����Z����
		wCnt = ((wCnt + wCntMax) - 1) % wCntMax;
	}
	else if ((input->IsPressed(KEY_INPUT_RSHIFT) || input->IsPressed(KEY_INPUT_LSHIFT)) &&
			 (input->IsTrigger(KEY_INPUT_DOWN)   || input->IsTrigger(KEY_INPUT_W)))
	{
		/// �E�F�[�u�J�E���g�����Z����
		wCnt = (wCnt + 1) % wCntMax;
	}
	else {}
}
