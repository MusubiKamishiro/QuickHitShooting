#pragma once
#include <string>
#include "Geometry.h"

///������`��̂��傤�ǂ����ʒu��Ԃ��N���X
class TrimString
{
private:
	// �Q�[����ʂ̃T�C�Y
	Vector2<int> ssize;
	// �t�H���g�T�C�Y
	int _fontSize;

public:
	TrimString();
	~TrimString();

	// �����̕��������ʂ̒����ɑ�������W��Ԃ�
	//@param name...�g�p���镶����
	//@return ������̍��[��x���W
	int GetStringCenterPosx(const std::string& name)const;
	// �����̕�������E�l�߂ɂ�����W��Ԃ�
	//@param name...�g�p���镶����
	//@param rpos...���������E�[�̍��W
	//@return ������̍��[��x���W
	int GetStringRightPosx(const std::string& name, const int& rpos)const;
	// ���݂̃t�H���g�T�C�Y��Ԃ�
	int GetFontSize()const;

	// �t�H���g�T�C�Y�̕ύX
	void ChangeFontSize(const int& fontSize);
};

