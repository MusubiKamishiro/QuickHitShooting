#include <DxLib.h>
#include "TargetDispTime.h"
#include "TargetAppearTime.h"

TargetDispTime::TargetDispTime()
{
}

TargetDispTime::~TargetDispTime()
{
}

void TargetDispTime::Update(int& wCnt, int& tCnt, 
							const unique_input& input, vec2_target& stageData)
{
	if (input->IsTrigger(KEY_INPUT_SPACE))
	{
		// �^�[�Q�b�g�ʒu�̏�����
		tCnt = 0;
		Stage::GetInstance().ChagneState(new TargetAppearTime());

		return;
	}

	ChangeTarget(tCnt, (int)stageData[wCnt].size(), input);
	ChangeWave(wCnt, (int)stageData.size(), input);
	DataConfig(wCnt, tCnt, input, stageData);

	Draw(wCnt, tCnt, stageData);
}

void TargetDispTime::Draw(const int& wCnt, const int& tCnt, const vec2_target stageData)
{
	/// �ݒ蒆�̏�ԕ`��
	SetFontSize(60);
	_text = "target DispTime Config";
	_drawPos.x = 0;
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x7fffd4);

	/// �E�F�[�u���̕`��
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

	/// �^�[�Q�b�g���̕\��
	SetFontSize(100);
	_text = "targetCount";
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Stage::GetInstance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = (Stage::GetInstance().GetScreenSize().y / 5) + (_strSize.y / 2);
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffffff);

	SetFontSize(120);
	_text = std::to_string(tCnt + 1) + " / " + std::to_string(stageData[wCnt].size());
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Stage::GetInstance().GetScreenSize().x / 2) - (_strSize.x / 2);
	_drawPos.y = (Stage::GetInstance().GetScreenSize().y / 3) + (_strSize.y / 2);
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffffff);

	/// �^�[�Q�b�g�̏o�����Ԃ̕\��
	SetFontSize(100);
	_text = "now Disp Time : " + std::to_string(stageData[wCnt][tCnt].dispTime);
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = (Stage::GetInstance().GetScreenSize().x / 5);
	_drawPos.y = Stage::GetInstance().GetScreenSize().y - (_strSize.y * 4);
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffffff);
}

void TargetDispTime::DataConfig(const int& wCnt, const int& tCnt, 
								const unique_input& input, vec2_target& stageData)
{
	if (input->IsTrigger(KEY_INPUT_UP))
	{
		stageData[wCnt][tCnt].dispTime += 10;
	}
	else if (input->IsTrigger(KEY_INPUT_DOWN))
	{
		stageData[wCnt][tCnt].dispTime = (stageData[wCnt][tCnt].dispTime >= 10
									   ?  stageData[wCnt][tCnt].dispTime - 10
									   :  stageData[wCnt][tCnt].dispTime);
	}
	else {}
}
