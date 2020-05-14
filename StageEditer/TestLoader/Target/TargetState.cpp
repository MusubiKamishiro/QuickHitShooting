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

void TargetState::DebugDraw(const int& wCnt, const int& tCnt, vec2_target stageData)
{
	int strWidth, strHeight;
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "現在のウェーブ : ", strlen("現在のウェーブ : "));
	DrawString(1000, 0, "現在のウェーブ : ", 0xffffff);
	DrawFormatString(1000 + strWidth, 0, 0xffffff, "%d", wCnt);

	GetDrawStringSize(&strWidth, &strHeight, nullptr, "現在のターゲット : ", strlen("現在のターゲット : "));
	DrawString(1000, 40, "現在のターゲット : ", 0xffffff);
	DrawFormatString(1000 + strWidth, 40, 0xffffff, "%d", tCnt);

	GetDrawStringSize(&strWidth, &strHeight, nullptr, "ターゲットタイプ :  ", strlen("ターゲットタイプ : "));
	DrawString(1000, 40 * 2, "ターゲットタイプ : ", 0xffffff);
	DrawFormatString(1000 + strWidth, 40 * 2, 0xffffff, "%d", stageData[wCnt][tCnt].type);

	GetDrawStringSize(&strWidth, &strHeight, nullptr, "出現する時間 : ", strlen("出現する時間 : "));
	DrawString(1000, 40 * 3, "出現する時間 : ", 0xffffff);
	DrawFormatString(1000 + strWidth, 40 * 3, 0xffffff, "%d", stageData[wCnt][tCnt].dispTime);

	GetDrawStringSize(&strWidth, &strHeight, nullptr, "消えるまでの時間 : ", strlen("消えるまでの時間 : "));
	DrawString(1000, 40 * 4, "消えるまでの時間 : ", 0xffffff);
	DrawFormatString(1000 + strWidth, 40 * 4, 0xffffff, "%d", stageData[wCnt][tCnt].appearTime);

	Vector2<int> pos;
	GetMousePoint(&pos.x, &pos.y);
	DrawBox(pos.x - (_boxSize / 2), pos.y - (_boxSize / 2),
			pos.x + (_boxSize / 2), pos.y + (_boxSize / 2),
			0x7fffd4, true);

	pos.x = stageData[wCnt][tCnt].pos.x;
	pos.y = stageData[wCnt][tCnt].pos.y;
	DrawBox(pos.x - (_boxSize / 2), pos.y - (_boxSize / 2),
			pos.x + (_boxSize / 2), pos.y + (_boxSize / 2),
			0xff4500, true);
}
