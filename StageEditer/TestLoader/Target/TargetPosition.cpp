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
		Stage::Instance().ChagneState(new TargetType());
		return;
	}
	ChangeTarget(tCnt, (int)stageData[wCnt].size(), input);
	ChangeWave(wCnt, (int)stageData.size(), input);
	DataConfig(wCnt, tCnt, input, stageData);
	
	Draw(wCnt, tCnt, stageData);
}

void TargetPosition::Draw(const int& wCnt, const int& tCnt, const vec2_target stageData)
{
	/// 設定中の状態表示
	SetFontSize(60);
	_text = "target Position Config";
	_drawPos.x = 0;
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x7fffd4);

	/// ウェーブ数の表示
	_text = "waveCount";
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Stage::Instance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffff9e);

	_text = std::to_string(wCnt + 1) + " / " + std::to_string(stageData.size());
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Stage::Instance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = _strSize.y;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffff9e);

	/// ターゲット数の表示
	_text = "targetCount";
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = Stage::Instance().GetScreenSize().x - (Stage::Instance().GetScreenSize().x / 10) - (_strSize.x / 2);
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffffff);

	_text = std::to_string(tCnt + 1) + " / " + std::to_string(stageData[wCnt].size());
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = Stage::Instance().GetScreenSize().x - (Stage::Instance().GetScreenSize().x / 10) - (_strSize.x / 2);
	_drawPos.y = _strSize.y;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffffff);

	/// ターゲット座標の表示
	DrawBox(stageData[wCnt][tCnt].pos.x, stageData[wCnt][tCnt].pos.y,
			stageData[wCnt][tCnt].pos.x + 50, stageData[wCnt][tCnt].pos.y + 50,
			0xffff0f, true);
}

void TargetPosition::DataConfig(const int& wCnt, const int& tCnt,
								const unique_input& input, vec2_target& stageData)
{
	if (input->IsMouseTrigger(MOUSE_INPUT_LEFT))
	{
		GetMousePoint(&stageData[wCnt][tCnt].pos.x, &stageData[wCnt][tCnt].pos.y);
	}
}
