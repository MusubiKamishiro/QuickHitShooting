#include "TargetState.h"

/// �ݒ肷��I�̕ύX
void TargetState::ChangeTarget(int& tCnt, const int& tCntMax, const unique_input& input)
{
	/// �ݒ蒆�̓I���ő�l�𒴂������̒���
	tCnt = (tCnt >= tCntMax - 1 ? tCntMax - 1 : tCnt);

	/// �ݒ肷��I�̐ؑ�
	if (!input->IsPressed(KEY_INPUT_RSHIFT) && !input->IsPressed(KEY_INPUT_LSHIFT) &&
		(input->IsTrigger(KEY_INPUT_DOWN)   ||  input->IsTrigger(KEY_INPUT_S)))
	{
		tCnt = (tCnt + 1) % tCntMax;
	}
	else if (!input->IsPressed(KEY_INPUT_RSHIFT) && !input->IsPressed(KEY_INPUT_LSHIFT) &&
			 (input->IsTrigger(KEY_INPUT_UP)	 || input->IsTrigger(KEY_INPUT_W)))
	{
		tCnt = ((tCnt + tCntMax) - 1) % tCntMax;
	}
	else {}
}

/// �ݒ肷��E�F�[�u�̕ύX
void TargetState::ChangeWave(int& wCnt, const int& wCntMax, const unique_input& input)
{
	/// �ݒ肷��E�F�[�u�̐ؑ�
	if ((input->IsPressed(KEY_INPUT_RSHIFT)	|| input->IsPressed(KEY_INPUT_LSHIFT)) &&
		(input->IsTrigger(KEY_INPUT_DOWN)   || input->IsTrigger(KEY_INPUT_S)))
	{
		wCnt = (wCnt + 1) % wCntMax;
	}
	else if ((input->IsPressed(KEY_INPUT_RSHIFT) || input->IsPressed(KEY_INPUT_LSHIFT)) &&
			 (input->IsTrigger(KEY_INPUT_UP)	 || input->IsTrigger(KEY_INPUT_W)))
	{
		wCnt = ((wCnt + wCntMax) - 1) % wCntMax;
	}
	else {}
}
