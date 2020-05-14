#include "TargetPosition.h"
#include "TargetType.h"

TargetPosition::TargetPosition()
{
}

TargetPosition::~TargetPosition()
{
}

void TargetPosition::Update(int& wCnt, int& tCnt, const unique_input& input, vec2_target& stageData)
{
	if (input->IsTrigger(KEY_INPUT_SPACE))
	{
		tCnt = 0;
		Stage::GetInstance().ChagneState(new TargetPosition());
		return;
	}
	ChangeTarget(tCnt, (int)stageData[wCnt].size(), input);
	ChangeWave(wCnt, (int)stageData.size(), input);
	DataConfig(wCnt, tCnt, input, stageData);
}

void TargetPosition::DataConfig(const int& wCnt, const int& tCnt,
								const unique_input& input, vec2_target& stageData)
{
	if (input->IsPressed(KEY_INPUT_D) ||
		input->IsPressed(KEY_INPUT_RIGHT))
	{
		/// 右移動
		stageData[wCnt][tCnt].pos.x += 2;
	}
	else if (input->IsPressed(KEY_INPUT_S) ||
			 input->IsPressed(KEY_INPUT_LEFT))
	{
		/// 左移動
		stageData[wCnt][tCnt].pos.x -= 2;
	}
	else if (input->IsPressed(KEY_INPUT_W) ||
			 input->IsPressed(KEY_INPUT_UP))
	{
		/// 上移動
		stageData[wCnt][tCnt].pos.y += 2;
	}
	else if (input->IsPressed(KEY_INPUT_S) ||
			 input->IsPressed(KEY_INPUT_DOWN))
	{
		/// 下移動
		stageData[wCnt][tCnt].pos.y -= 2;
	}
	else{}
}
