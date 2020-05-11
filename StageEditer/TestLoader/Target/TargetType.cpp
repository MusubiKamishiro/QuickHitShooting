#include <DxLib.h>
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
		// ターゲット位置の初期化
		tCnt = 0;
		Stage::GetInstance().ChagneState(new TargetDispTime());
		
		return;
	}

	ChangeTarget(tCnt, (int)stageData[wCnt].size(), input);
	ChangeWave(wCnt, (int)stageData.size(), input);
	SetPosition(wCnt, tCnt, input, stageData);
	DataConfig(wCnt, tCnt, input, stageData);
	//// フォントサイズの変更を忘れないようにしておく
	DrawString(0, 0, "的IDの設定", 0xffffff);

	DebugDraw(wCnt, tCnt, stageData);
}

void TargetType::DataConfig(const int& wCnt, const int& tCnt, 
							const unique_input& input, vec2_target& stageData)
{
	// 的IDの最大値の取得
	char idMax = static_cast<char>(TargetID::MAX);

	if (input->IsTrigger(KEY_INPUT_UP))
	{
		/// 的IDの加算
		stageData[wCnt][tCnt].type = (stageData[wCnt][tCnt].type + 1) % idMax;
	}
	else if (input->IsTrigger(KEY_INPUT_DOWN))
	{
		/// 的IDの減算
		stageData[wCnt][tCnt].type = ((stageData[wCnt][tCnt].type + idMax) - 1) % idMax;
	}
	else {}
}
