#include <DxLib.h>
#include "../Stage.h"
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
	
	DataConfig(wCnt, tCnt, input, stageData);
	//// フォントサイズの変更を忘れないようにしておく

	Draw(wCnt, tCnt, stageData);

	DebugDraw(wCnt, tCnt, stageData);
}

void TargetType::Draw(const int& wCnt, const int& tCnt, const vec2_target stageData)
{
	SetFontSize(40);
	Vector2<int> drawPos;
	int strWidth, strHeight;
	std::string text;

	/// 現在、設定している的の状態描画
	text = "的IDの設定";
	GetDrawStringSize(&strWidth, &strHeight, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((Stage::GetInstance().GetScreenSize().x / 4) - (strWidth / 2), 0,
				text.c_str(), 0xffffff);

	/// ウェーブ数の設定
	text = "現在のウェーブ数 : " + std::to_string(wCnt);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, text.c_str(), strlen(text.c_str()));

	drawPos.x = Stage::GetInstance().GetScreenSize().x -
			   (Stage::GetInstance().GetScreenSize().x / 8) - (strWidth / 2);
	drawPos.y = strHeight / 2;
	
	DrawString(drawPos.x, drawPos.y, text.c_str(), 0xffffff);
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
