#include <cmath>
#include "TargetPosition.h"
#include "TargetType.h"

TargetPosition::TargetPosition() : _alphaMax(256)
{
	_alpha = 0;
	_idImage[static_cast<int>(TargetID::NORMAL)]	= LoadGraph("img/nEnemy.png");
	_idImage[static_cast<int>(TargetID::SPECIAL)]	= LoadGraph("img/dEnemy.png");
	_idImage[static_cast<int>(TargetID::DEDUCTION)] = LoadGraph("img/sEnemy.png");
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
	SetFontSize(48);
	_text = "Target Position Config";
	_drawPos.x = 0;
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x228b22);

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

	/// 全ての的表示
	for (int i = 0; i < stageData[wCnt].size(); ++i)
	{
		DrawExtendGraph(stageData[wCnt][i].pos.x, stageData[wCnt][i].pos.y,
						stageData[wCnt][i].pos.x + _boxSize, stageData[wCnt][i].pos.y + _boxSize,
						_idImage[stageData[wCnt][i].type], true);
	}

	/// 設定中の座標
	_alpha = (_alpha + 8) % (_alphaMax * 2);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, abs(_alpha - _alphaMax));

	DrawCircle(stageData[wCnt][tCnt].pos.x + _boxSize / 2, 
			   stageData[wCnt][tCnt].pos.y + _boxSize / 2, _boxSize / 2, 
			   0xffffd1, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}