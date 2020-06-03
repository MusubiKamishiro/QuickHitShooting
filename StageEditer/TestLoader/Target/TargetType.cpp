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
						const unique_input& input, std::vector<vec_target>& stageData)
{
	if (input->IsTrigger(KEY_INPUT_SPACE))
	{
		Editer::Instance().ChagneState(new TargetDispTime());
		return;
	}
	ChangeWave(wCnt, (int)stageData.size(), input);
	ChangeTarget(tCnt, (int)stageData[wCnt].size(), input);
	DataConfig(wCnt, tCnt, input, stageData);
}

void TargetType::DataConfig(const int& wCnt, const int& tCnt,
							const unique_input& input, std::vector<vec_target>& stageData)
{
	// 的IDの最大値の取得
	char idMax = static_cast<char>(TargetID::MAX);

	/// 的IDの変更用
	if (input->IsTrigger(KEY_INPUT_RIGHT) || input->IsTrigger(KEY_INPUT_D))
	{
		stageData[wCnt][tCnt].type = (stageData[wCnt][tCnt].type + 1) % idMax;
	}
	else if (input->IsTrigger(KEY_INPUT_LEFT) || input->IsTrigger(KEY_INPUT_A))
	{
		stageData[wCnt][tCnt].type = ((stageData[wCnt][tCnt].type + idMax) - 1) % idMax;
	}
	else {}
}

/// ステージデータの描画(的ID)
void TargetType::Draw(const int& wCnt, const int& tCnt, const std::vector<vec_target> stageData)
{
	/// 下地
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(0, 0, Editer::Instance().GetScreenSize().x, Editer::Instance().GetScreenSize().y, 
			0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	/// 現在のモード
	SetFontSize(48);
	_text	   = "ID Config";
	_drawPos.x = 0;
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x228b22);
	
	/// ウェーブ数
	_text	   = "Wave Count";
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = 0;
	_drawPos.y = _strSize.y * 2;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x00008b);

	_text	   = std::to_string(wCnt + 1) + " / " + std::to_string(stageData.size());
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = _strSize.x / 2;
	_drawPos.y = _strSize.y * 3;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x00008b);

	/// 的数
	SetFontSize(80);
	_text	   = "Target Count";
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Editer::Instance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = (Editer::Instance().GetScreenSize().y / 5) + (_strSize.y / 2);
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x000000);

	_text	   = std::to_string(tCnt + 1) + " / " + std::to_string(stageData[wCnt].size());
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Editer::Instance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = (Editer::Instance().GetScreenSize().y / 3) + (_strSize.y / 2);
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x000000);

	/// 現在のID
	_text = "Target: ";
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Editer::Instance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = (Editer::Instance().GetScreenSize().y / 2) + _strSize.y;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x191970);
	
	_drawPos.x = (Editer::Instance().GetScreenSize().x / 2) + (_strSize.x / 2);
	DrawExtendGraph(_drawPos.x, _drawPos.y - 10,
					_drawPos.x + 100, _drawPos.y + 90,
					_imageID[stageData[wCnt][tCnt].type], true);

	/// 全てのIDの表示
	SetFontSize(54);
	int configColor = 0;			/// 設定中の色
	int id;
	for (int i = 0; i < stageData[wCnt].size(); ++i)
	{
		id = stageData[wCnt][i].type;
		configColor = (i == tCnt ? 0xff0000 : 0x000000);
		_text	    = std::to_string(i + 1) + ": 　";
		GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
		_drawPos.x = Editer::Instance().GetScreenSize().x - _strSize.x;
		_drawPos.y = (_strSize.y * i);

		DrawString(_drawPos.x, _drawPos.y, _text.c_str(), configColor);
		DrawExtendGraph(Editer::Instance().GetScreenSize().x - 54, (54 * i),
						Editer::Instance().GetScreenSize().x, (54 * (i + 1)),
						_imageID[id], true);
	}
}