#pragma once
#include <string>

// �e�̎��
enum class GunType
{

};

// �e�̕���(������)
enum class ShootType
{

};

// �e���\������K�v�ϐ�
struct GunStatus
{
	std::string name;			// �e��
	GunType gunType;			// �e�̎��
	ShootType shootType;		// �e�̕���(������)
	int maxBullets;				// �ő�̑��e��
	int remainingBullets;		// �c��̑��e��
	int maxBulletsInMagazine;	// �}�K�W�����̍ő�e��
	int BulletsInMagazine;		// �c��̃}�K�W�����̒e��
};

class Gun
{
private:
	GunStatus _gun;

public:
	Gun(const GunStatus& gunState);
	~Gun();

	///�}�K�W���ɒe�����߂�
	///�}�K�W�����ɒe���c���Ă��Ă��\
	void Reload();

	///�e�����˂��ꂽ��true��Ԃ�
	///@retval true ������
	///@retval false �}�K�W�����ɒe���Ȃ�
	bool Shot();

	///�c�e�����̕`��
	void Draw();
};

