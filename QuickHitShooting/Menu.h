#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "Geometry.h"

class Peripheral;
class CollisionDetector;

// ���j���[�ɕK�v�ȏ��
struct MenuData
{
	MenuData() : rect(0, 0, 0, 0), img(-1) {};
	MenuData(Rect rect, int img) : rect(rect), img(img) {};
	Rect rect;		// ���W�ƕ���
	int img;		// �摜
};

class Menu
{
private:
	///���j���[�e�[�u��
	///@param string �{�^����
	///@param int ���j���[�f�[�^
	std::map<std::string, MenuData> _menuTable;

	// �����蔻��
	std::shared_ptr<CollisionDetector> _cd;

public:
	Menu();
	~Menu();

	///���j���[���X�g�ɒǉ�
	///@param name	���j���[��
	///@param pos	�摜�̕\�����W(���S)
	///@param size	�摜�̕���
	///@param img	�摜�n���h��
	void AddMenuList(const std::string& name, const Vector2<int>& pos, const Size& size, const int& img);

	///�����̃��j���[���N���b�N���������m�F����
	///@param name	�m�F���������j���[��
	///@retval true	������, false �����ĂȂ�
	bool CheckCrick(const std::string& name, const Peripheral& p);

	void Update(const Peripheral &p);
	
	///���j���[�̕`��
	///�ǉ����ꂽ���̑S�ĕ`�悷��
	void Draw();
};

