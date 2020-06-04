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
	int oldMouseState;	// 1�t���[���O�̃}�E�X�̓��͏��

	int pointImg;		// �}�E�X�|�C���^�[�̉摜
	Vector2<int> mousePos;	// �}�E�X�̍��W

public:
	Peripheral();
	~Peripheral();

	///���͏��̍X�V(���t���[��)
	void Update();

	///���݂̉�����Ԃ̌��o
	///@param cmd	���ׂ����R�}���h
	///@retval true	�����Ă�, false �����ĂȂ�
	bool IsPressing(const int& cmd)const;

	///���݂̃g���K�[���(�������u��)�̌��o
	///@param cmd	���ׂ����R�}���h
	///@retval true	������, false �����ĂȂ�
	bool IsTrigger(const int& cmd)const;

	///�}�E�X�̍��W��Ԃ�
	///@return �}�E�X�̍��W
	Vector2<int> GetMousePos()const;

	void Draw();
};

