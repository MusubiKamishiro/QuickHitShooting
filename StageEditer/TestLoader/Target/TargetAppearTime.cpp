#include <DxLib.h>
#include "TargetAppearTime.h"
#include "TargetPosition.h"

TargetAppearTime::TargetAppearTime()
{
}

TargetAppearTime::~TargetAppearTime()
{
}

void TargetAppearTime::Update(int& wCnt, int& tCnt, 
							  const unique_input& input, vec2_target& stageData)
{
	if (input->IsTrigger(KEY_INPUT_SPACE))
	{
		// ターゲット位置の初期化
		tCnt = 0;
		Stage::GetInstance().ChagneState(new TargetPosition());
		return;
	}

	ChangeTarget(tCnt, (int)stageData[wCnt].size(), input);
	ChangeWave(wCnt, (int)stageData.size(), input);
	DataConfig(wCnt, tCnt, input, stageData);

	DrawString(0, 0, "的が消えるまでの時間設定", 0xffffff);

	DebugDraw(wCnt, tCnt, stageData);
}

void TargetAppearTime::Draw(const int& wCnt, const int& tCnt, const vec2_target stageData)
{
}

void TargetAppearTime::DataConfig(const int& wCnt, const int& tCnt, const unique_input& input, vec2_target& stageData)
{
	if (input->IsTrigger(KEY_INPUT_UP))
	{
		stageData[wCnt][tCnt].appearTime += 10;
	}
	else if (input->IsTrigger(KEY_INPUT_DOWN))
	{
		stageData[wCnt][tCnt].appearTime = (stageData[wCnt][tCnt].appearTime >= 10
										 ?  stageData[wCnt][tCnt].appearTime - 10
										 :  stageData[wCnt][tCnt].appearTime);
	}
	else {}
}
