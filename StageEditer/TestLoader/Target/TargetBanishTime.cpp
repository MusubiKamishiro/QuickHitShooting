#include <DxLib.h>
#include "TargetBanishTime.h"
#include "TargetPosition.h"

TargetBanishTime::TargetBanishTime()
{
}

TargetBanishTime::~TargetBanishTime()
{
}

void TargetBanishTime::Update(int& wCnt, int& tCnt, 
							  const unique_input& input, std::vector<vec_target>& stageData)
{
	if (input->IsTrigger(KEY_INPUT_SPACE))
	{
		// ターゲット位置の初期化
		tCnt = 0;
		wCnt = 0;
		Editer::Instance().ChagneState(new TargetPosition());
		return;
	}

	ChangeWave(wCnt, (int)stageData.size(), input);
	ChangeTarget(tCnt, (int)stageData[wCnt].size(), input);
	DataConfig(wCnt, tCnt, input, stageData);
}

void TargetBanishTime::DataConfig(const int& wCnt, const int& tCnt, 
								  const unique_input& input, std::vector<vec_target>& stageData)
{
	if (input->IsTrigger(KEY_INPUT_RIGHT) || input->IsTrigger(KEY_INPUT_D))
	{
		stageData[wCnt][tCnt].banishTime += 10;
	}
	else if (input->IsTrigger(KEY_INPUT_LEFT) || input->IsTrigger(KEY_INPUT_A))
	{
		stageData[wCnt][tCnt].banishTime = (stageData[wCnt][tCnt].banishTime >= 10
			? stageData[wCnt][tCnt].banishTime - 10
			: stageData[wCnt][tCnt].banishTime);
	}
	else {}
}

/// ステージデータの描画(消えるまでの時間)
void TargetBanishTime::Draw(const int& wCnt, const int& tCnt, const std::vector<vec_target> stageData)
{
	/// 現在のモード
	SetFontSize(60);
	_text = "Target BanishTime Config";
	_drawPos.x = 0;
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x7fffd4);

	/// ウェーブ数
	_text = "Wave Count";
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = 0;
	_drawPos.y = _strSize.y * 2;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffff9e);

	_text = std::to_string(wCnt + 1) + " / " + std::to_string(stageData.size());
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = _strSize.x / 2;
	_drawPos.y = _strSize.y * 3;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffff9e);

	/// 的数
	SetFontSize(100);
	_text = "Target Count";
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Editer::Instance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = (Editer::Instance().GetScreenSize().y / 5) + (_strSize.y / 2);
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffffff);

	_text = std::to_string(tCnt + 1) + " / " + std::to_string(stageData[wCnt].size());
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Editer::Instance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = (Editer::Instance().GetScreenSize().y / 3) + (_strSize.y / 2);
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffffff);

	/// 消えるまでの時間
	_text	   = "Now Banish Time : " + std::to_string(stageData[wCnt][tCnt].banishTime);
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Editer::Instance().GetScreenSize().x / 5);
	_drawPos.y = (Editer::Instance().GetScreenSize().y / 2) + _strSize.y;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffffff);

	/// 全ての消えるまでの時間
	SetFontSize(60);
	int configColor = 0;			/// 設定中の色
	for (int i = 0; i < stageData[wCnt].size(); ++i)
	{
		configColor = (i == tCnt ? 0xffff00 : 0xffffff);
		_text	   = std::to_string(i + 1) + " : " + std::to_string(stageData[wCnt][i].banishTime);
		GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
		_drawPos.x = Editer::Instance().GetScreenSize().x - _strSize.x;
		_drawPos.y = (_strSize.y * i);

		DrawString(_drawPos.x, _drawPos.y, _text.c_str(), configColor);
	}
}