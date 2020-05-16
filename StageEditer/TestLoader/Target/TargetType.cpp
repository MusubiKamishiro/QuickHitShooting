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
		// �^�[�Q�b�g�ʒu�̏�����
		tCnt = 0;
		Stage::GetInstance().ChagneState(new TargetDispTime());
		
		return;
	}

	ChangeTarget(tCnt, (int)stageData[wCnt].size(), input);
	ChangeWave(wCnt, (int)stageData.size(), input);
	
	DataConfig(wCnt, tCnt, input, stageData);
	//// �t�H���g�T�C�Y�̕ύX��Y��Ȃ��悤�ɂ��Ă���

	Draw(wCnt, tCnt, stageData);

	//DebugDraw(wCnt, tCnt, stageData);
}

void TargetType::Draw(const int& wCnt, const int& tCnt, const vec2_target stageData)
{
	/// �ݒ蒆�̏�ԕ`��
	SetFontSize(60);
	_text	  = "�IID�̐ݒ�";
	_drawPos.x = Stage::GetInstance().GetScreenSize().x / 9;
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0x7fffd4);
	
	/// �E�F�[�u���̐ݒ�
	_text = "���݂̃E�F�[�u�� : " + std::to_string(wCnt);
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _text.c_str(), strlen(_text.c_str()));
	_drawPos.x = Stage::GetInstance().GetScreenSize().x - _strSize.x - (_strSize.x / 3);
	_drawPos.y = 0;
	DrawString(_drawPos.x, _drawPos.y, _text.c_str(), 0xffff9e);

	/// ���݂̃^�[�Q�b�g�̕\��
	SetFontSize(100);
	_text = "�ݒ肵�Ă���^�[�Q�b�g";
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
	// �IID�̍ő�l�̎擾
	char idMax = static_cast<char>(TargetID::MAX);

	if (input->IsTrigger(KEY_INPUT_UP))
	{
		/// �IID�̉��Z
		stageData[wCnt][tCnt].type = (stageData[wCnt][tCnt].type + 1) % idMax;
	}
	else if (input->IsTrigger(KEY_INPUT_DOWN))
	{
		/// �IID�̌��Z
		stageData[wCnt][tCnt].type = ((stageData[wCnt][tCnt].type + idMax) - 1) % idMax;
	}
	else {}
}
