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
	MenuData() : rect(0, 0, 0, 0), useRect(0, 0, 0, 0), img(-1), flag(false) {};
	MenuData(int img, Rect rect, Rect userect = Rect()) : img(img), rect(rect), useRect(userect), flag(false) {};
	int img;		// �摜
	Rect rect;		// ���W�ƕ���
	Rect useRect;	// �摜���̎g�p����͈�
	bool flag;		// �J�[�\����rect���ɓ����Ă��true�Ԃ�
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

	int img;
	int size;

public:
	Menu();
	~Menu();

	///���j���[���X�g�ɒǉ�
	///@param name	���j���[��
	///@param ltPos	�摜��\�����鍶����W
	///@param rbPos	�摜��\������E�����W
	///@param size	�摜�̕���
	///@param img	�摜�n���h��
	void AddMenuList(const std::string& name, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const int& img);
	///@param imgRect	�摜���̎g�p����͈�
	void AddMenuList(const std::string& name, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const Rect& imgRect, const int& img);

	///�����̃��j���[���N���b�N���������m�F����
	///@param name	�m�F���������j���[��
	///@retval true	������, false �����ĂȂ�
	bool CheckClick(const std::string& name, const Peripheral& p);

	void Update(const Peripheral &p);
	
	///���j���[�̕`��
	///�ǉ����ꂽ���̑S�ĕ`�悷��
	void Draw();
};

