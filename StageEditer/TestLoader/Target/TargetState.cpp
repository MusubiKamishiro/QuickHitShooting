#include "TargetState.h"

void TargetState::ChangeTarget(int& tCnt, const int& tCntMax, const unique_input& input)
{
	if (!input->IsPressed(KEY_INPUT_RSHIFT) && !input->IsPressed(KEY_INPUT_LSHIFT) &&
		(input->IsTrigger(KEY_INPUT_LEFT)	||  input->IsTrigger(KEY_INPUT_A)))
	{
		/// �^�[�Q�b�g�J�E���g�����Z����
		tCnt = ((tCnt + tCntMax) - 1) % tCntMax;
	}
	else if (!input->IsPressed(KEY_INPUT_RSHIFT) && !input->IsPressed(KEY_INPUT_LSHIFT) &&
			 (input->IsTrigger(KEY_INPUT_RIGHT)  ||  input->IsTrigger(KEY_INPUT_D)))
	{
		/// �^�[�Q�b�g�J�E���g�����Z����
		tCnt = (tCnt + 1) % tCntMax;
	}
	else {}
}

void TargetState::ChangeWave(int& wCnt, const int& wCntMax, const unique_input& input)
{
	if ((input->IsPressed(KEY_INPUT_RSHIFT) || input->IsPressed(KEY_INPUT_LSHIFT)) &&
		(input->IsTrigger(KEY_INPUT_LEFT)	|| input->IsTrigger(KEY_INPUT_A)))
	{
		/// �E�F�[�u�J�E���g�����Z����
		wCnt = ((wCnt + wCntMax) - 1) % wCntMax;
	}
	else if ((input->IsPressed(KEY_INPUT_RSHIFT) || input->IsPressed(KEY_INPUT_LSHIFT)) &&
			 (input->IsTrigger(KEY_INPUT_RIGHT)  || input->IsTrigger(KEY_INPUT_D)))
	{
		/// �E�F�[�u�J�E���g�����Z����
		wCnt = (wCnt + 1) % wCntMax;
	}
	else {}
}
