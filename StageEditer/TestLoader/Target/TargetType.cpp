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
	
	DataConfig(wCnt, tCnt, input, stageData);
	//// フォントサイズの変更を忘れないようにしておく

	Draw(wCnt, tCnt, stageData);

	//DebugDraw(wCnt, tCnt, stageData);
}

void TargetType::Draw(const int& wCnt, const int& tCnt, const vec2_target stageData)
{
	/// 設定中の状態描画
	SetFontSize(60);
	_text	  = "的IDの設定";
	_drawPos.x = Stage::GetInstance().GetScreenSize().x / 9;
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x7fffd4);
	
	/// ウェーブ数の設定
	_text = "現在のウェーブ数 : " + std::to_string(wCnt);
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = Stage::GetInstance().GetScreenSize().x - _strSize.x - (_strSize.x / 3);
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffff9e);

	/// 現在のターゲットの表示
	SetFontSize(100);
	_text = "設定しているターゲット";
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Stage::GetInstance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = (Stage::GetInstance().GetScreenSize().y / 5) - (_strSize.y / 2);
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffffff);

	SetFontSize(150);
	_text	  = std::to_string(stageData[wCnt][tCnt].type + 1) + " / " + 
				std::to_string(stageData[wCnt].size());
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Stage::GetInstance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = (Stage::GetInstance().GetScreenSize().y / 3) - (_strSize.y / 3);
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffffff);
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
