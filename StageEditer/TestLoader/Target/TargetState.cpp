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
