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

void TargetPosition::DataConfig(const int& wCnt, const int& tCnt,const unique_input& input, std::vector<vec_target>& stageData)
{
	if (input->IsMouseTrigger(MOUSE_INPUT_LEFT))
	{
		Vector2<int> mPos;
		GetMousePoint(&mPos.x, &mPos.y);
		mPos -= Vector2<int>(_targetSize / 2, _targetSize / 2);
		if (CheckTargetRange(mPos))
		{
			stageData[wCnt][tCnt].pos = mPos;
		}
	}
}

/// 的を配置する位置が範囲内であるかの判定取得用
bool TargetPosition::CheckTargetRange(const Vector2<int>& pos) const
{
	return (pos.x >= 0  && pos.x < Editer::Instance().GetScreenSize().x &&
			pos.y >= 96 && pos.y < Editer::Instance().GetScreenSize().y - 100 - (_targetSize / 2));
}

void TargetPosition::Draw(const int& wCnt, const int& tCnt, const std::vector<vec_target> stageData)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(0, 0, Editer::Instance().GetScreenSize().x, 96,
		    0xffffff, true);
	DrawBox(0, 96, Editer::Instance().GetScreenSize().x, Editer::Instance().GetScreenSize().y - 100,
			0x00ff00, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	/// 現在のモード
	SetFontSize(48);
	_text = "Position Config";
	_drawPos.x = 0;
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x228b22);

	/// ウェーブ数
	_text = "Wave Count";
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Editer::Instance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x00008b);

	_text = std::to_string(wCnt + 1) + " / " + std::to_string(stageData.size());
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Editer::Instance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = _strSize.y;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x00008b);

	/// 的数
	_text = "Target Count";
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = Editer::Instance().GetScreenSize().x - _strSize.x;
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x000000);

	_text = std::to_string(tCnt + 1) + " / " + std::to_string(stageData[wCnt].size());
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = Editer::Instance().GetScreenSize().x - (Editer::Instance().GetScreenSize().x / 10) - (_strSize.x / 2);
	_drawPos.y = _strSize.y;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x000000);

	/// 全ての的表示
	for (int i = 0; i < stageData[wCnt].size(); ++i)
	{
		DrawExtendGraph(stageData[wCnt][i].pos.x, stageData[wCnt][i].pos.y,
						stageData[wCnt][i].pos.x + _targetSize, stageData[wCnt][i].pos.y + _targetSize,
						_imageID[stageData[wCnt][i].type], true);
	}

	/// 設定中の座標
	_alpha = (_alpha + 10) % (_alphaMax * 2);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, abs(_alpha - _alphaMax));
	DrawCircle(stageData[wCnt][tCnt].pos.x + _targetSize / 2, 
			   stageData[wCnt][tCnt].pos.y + _targetSize / 2, _targetSize / 2, 
			   0xffffd1, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}