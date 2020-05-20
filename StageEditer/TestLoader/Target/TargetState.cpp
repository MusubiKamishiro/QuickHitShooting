#include "TargetState.h"

void TargetState::ChangeTarget(int& tCnt, const int& tCntMax, const unique_input& input)
{
	if (!input->IsPressed(KEY_INPUT_RSHIFT) && !input->IsPressed(KEY_INPUT_LSHIFT) &&
		(input->IsTrigger(KEY_INPUT_UP)		||  input->IsTrigger(KEY_INPUT_S)))
	{
		/// ターゲットカウントを減算する
		tCnt = ((tCnt + tCntMax) - 1) % tCntMax;
	}
	else if (!input->IsPressed(KEY_INPUT_RSHIFT) && !input->IsPressed(KEY_INPUT_LSHIFT) &&
			 (input->IsTrigger(KEY_INPUT_DOWN)   ||  input->IsTrigger(KEY_INPUT_W)))
	{
		/// ターゲットカウントを加算する
		tCnt = (tCnt + 1) % tCntMax;
	}
	else {}
}

void TargetState::ChangeWave(int& wCnt, const int& wCntMax, const unique_input& input)
{
	if ((input->IsPressed(KEY_INPUT_RSHIFT) || input->IsPressed(KEY_INPUT_LSHIFT)) &&
		(input->IsTrigger(KEY_INPUT_UP)		|| input->IsTrigger(KEY_INPUT_S)))
	{
		/// ウェーブカウントを減算する
		wCnt = ((wCnt + wCntMax) - 1) % wCntMax;
	}
	else if ((input->IsPressed(KEY_INPUT_RSHIFT) || input->IsPressed(KEY_INPUT_LSHIFT)) &&
			 (input->IsTrigger(KEY_INPUT_DOWN)   || input->IsTrigger(KEY_INPUT_W)))
	{
		/// ウェーブカウントを加算する
		wCnt = (wCnt + 1) % wCntMax;
	}
	else {}
}
