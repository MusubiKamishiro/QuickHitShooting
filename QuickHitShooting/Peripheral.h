#pragma once
#include <vector>
#include <array>
#include <map>
#include <string>
#include "Geometry.h"

// ���Ӌ@��(�}�E�X)�̓��͂𐧌�
class Peripheral
{
private:
	int mouseState;		// �}�E�X�̓��͏��
	int oldMouseState;

	Vector2<int> mousePos;

public:
	Peripheral();
	~Peripheral();

	// ���͏��̍X�V(���t���[��)
	void Update();

	// ���݂̉�����Ԃ̌��o
	//@param cmd	���ׂ����R�}���h
	//@retval true	�����Ă�, false �����ĂȂ�
	bool IsPressing(const int& cmd)const;

	// ���݂̃g���K�[���(�������u��)�̌��o
	//@param cmd	���ׂ����R�}���h
	//@retval true	������, false �����ĂȂ�
	bool IsTrigger(const int& cmd)const;

	void DebugDraw();
};

