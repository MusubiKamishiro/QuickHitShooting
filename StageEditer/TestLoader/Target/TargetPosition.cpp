#include <cmath>
#include "TargetPosition.h"
#include "TargetType.h"

TargetPosition::TargetPosition() : _alphaMax(256)
{
	_alpha = 0;
}

TargetPosition::~TargetPosition()
{
}

void TargetPosition::Update(int& wCnt, int& tCnt, const unique_input& input, std::vector<vec_target>& stageData)
{
	if (input->IsTrigger(KEY_INPUT_SPACE))
	{
		tCnt = 0;
		wCnt = 0;
		Editer::Instance().ChagneState(new TargetType());
		return;
	}
	ChangeWave(wCnt, (int)stageData.size(), input);
	ChangeTarget(tCnt, (int)stageData[wCnt].size(), input);
	DataConfig(wCnt, tCnt, input, stageData);
}

void TargetPosition::DataConfig(const int& wCnt, const int& tCnt,
	const unique_input& input, std::vector<vec_target>& stageData)
{
	if (input->IsMouseTrigger(MOUSE_INPUT_LEFT))
	{
		GetMousePoint(&stageData[wCnt][tCnt].pos.x, &stageData[wCnt][tCnt].pos.y);
	}
}

void TargetPosition::Draw(const int& wCnt, const int& tCnt, const std::vector<vec_target> stageData)
{
	/// 現在のモード
	SetFontSize(60);
	_text = "Target Position Config";
	_drawPos.x = 0;
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x7fffd4);

	/// ウェーブ数
	_text = "Wave Count";
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Editer::Instance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffff9e);

	_text = std::to_string(wCnt + 1) + " / " + std::to_string(stageData.size());
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Editer::Instance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = _strSize.y;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffff9e);

	/// 的数
	_text = "Target Count";
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = Editer::Instance().GetScreenSize().x - (Editer::Instance().GetScreenSize().x / 10) - (_strSize.x / 2);
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffffff);

	_text = std::to_string(tCnt + 1) + " / " + std::to_string(stageData[wCnt].size());
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = Editer::Instance().GetScreenSize().x - (Editer::Instance().GetScreenSize().x / 10) - (_strSize.x / 2);
	_drawPos.y = _strSize.y;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffffff);

	/// 全ての座標
	bool configTarget = true;			/// 設定中の色
	int thickSize	  = 6;
	for (int i = 0; i < stageData[wCnt].size(); ++i)
	{
		configTarget = (i == tCnt ? true : false);
		/// ターゲット座標の表示
		DrawBoxAA(stageData[wCnt][i].pos.x, stageData[wCnt][i].pos.y,
				  stageData[wCnt][i].pos.x + 50, stageData[wCnt][i].pos.y + 50,
				  _typeColor[stageData[wCnt][i].type], 
				  configTarget, thickSize);
	}

	/// 設定中の座標
	_alpha = (_alpha + 8) % (_alphaMax * 2);
	thickSize /= 2;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, abs(_alpha - _alphaMax));

	DrawBox(stageData[wCnt][tCnt].pos.x - thickSize, stageData[wCnt][tCnt].pos.y - thickSize,
		stageData[wCnt][tCnt].pos.x + 50 + thickSize, stageData[wCnt][tCnt].pos.y + 50 + thickSize,
		0xffffff, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}