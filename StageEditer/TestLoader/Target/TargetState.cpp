#include "TargetState.h"

/// 設定する的の変更
void TargetState::ChangeTarget(int& tCnt, const int& tCntMax, const unique_input& input)
{
	/// 設定中の的が最大値を超えた時の調整
	tCnt = (tCnt >= tCntMax - 1 ? tCntMax - 1 : tCnt);

	/// 設定する的の切替
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

/// 設定するウェーブの変更
void TargetState::ChangeWave(int& wCnt, const int& wCntMax, const unique_input& input)
{
	/// 設定するウェーブの切替
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
