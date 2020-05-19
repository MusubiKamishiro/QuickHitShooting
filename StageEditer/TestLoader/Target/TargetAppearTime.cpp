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
		wCnt = 0;
		Stage::Instance().ChagneState(new TargetPosition());
		return;
	}

	ChangeTarget(tCnt, (int)stageData[wCnt].size(), input);
	ChangeWave(wCnt, (int)stageData.size(), input);
	DataConfig(wCnt, tCnt, input, stageData);

	Draw(wCnt, tCnt, stageData);
}

void TargetAppearTime::Draw(const int& wCnt, const int& tCnt, const vec2_target stageData)
{
	/// 設定中の状態描画
	SetFontSize(60);
	_text = "target AppearTime Config";
	_drawPos.x = 0;
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x7fffd4);

	/// ウェーブ数の設定
	_text = "waveCount";
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = 0;
	_drawPos.y = _strSize.y * 2;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffff9e);

	_text = std::to_string(wCnt + 1) + " / " + std::to_string(stageData.size());
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = _strSize.x / 2;
	_drawPos.y = _strSize.y * 3;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffff9e);

	/// ターゲット数の描画
	SetFontSize(100);
	_text = "targetCount";
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Stage::Instance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = (Stage::Instance().GetScreenSize().y / 5) + (_strSize.y / 2);
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffffff);

	_text = std::to_string(tCnt + 1) + " / " + std::to_string(stageData[wCnt].size());
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Stage::Instance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = (Stage::Instance().GetScreenSize().y / 3) + (_strSize.y / 2);
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffffff);

	/// ターゲットが出現してから消えるまでの時間表示
	_text	   = "now Appear Time : " + std::to_string(stageData[wCnt][tCnt].appearTime);
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Stage::Instance().GetScreenSize().x / 5);
	_drawPos.y = (Stage::Instance().GetScreenSize().y / 2) + _strSize.y;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffffff);

	/// 全てのターゲットの出現してから消えるまでの時間表示
	SetFontSize(60);
	int configColor = 0;			/// 設定中の色
	for (int i = 0; i < stageData[wCnt].size(); ++i)
	{
		configColor = (i == tCnt ? 0xffff00 : 0xffffff);
		_text	   = std::to_string(i + 1) + " : " + std::to_string(stageData[wCnt][i].appearTime);
		GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
		_drawPos.x = Stage::Instance().GetScreenSize().x - _strSize.x;
		_drawPos.y = (_strSize.y * i);

		DrawString(_drawPos.x, _drawPos.y, _text.c_str(), configColor);
	}
}

void TargetAppearTime::DataConfig(const int& wCnt, const int& tCnt, const unique_input& input, vec2_target& stageData)
{
	if (input->IsTrigger(KEY_INPUT_RIGHT) || input->IsTrigger(KEY_INPUT_D))
	{
		stageData[wCnt][tCnt].appearTime += 10;
	}
	else if (input->IsTrigger(KEY_INPUT_LEFT) || input->IsTrigger(KEY_INPUT_A))
	{
		stageData[wCnt][tCnt].appearTime = (stageData[wCnt][tCnt].appearTime >= 10
										 ?  stageData[wCnt][tCnt].appearTime - 10
										 :  stageData[wCnt][tCnt].appearTime);
	}
	else {}
}
