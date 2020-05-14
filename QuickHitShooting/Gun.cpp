#include "Gun.h"
#include <DxLib.h>

Gun::Gun(const GunStatus& gunState)
{
	_gun = gunState;
}

Gun::~Gun()
{
}

void Gun::Reload()
{
	int magazine = _gun.maxBulletsInMagazine - _gun.BulletsInMagazine;

	if (_gun.remainingBullets < magazine)
	{
		magazine = _gun.remainingBullets;
	}

	_gun.remainingBullets -= magazine;
	_gun.BulletsInMagazine += magazine;
}

bool Gun::Shot()
{
	if (_gun.BulletsInMagazine > 0)
	{
		_gun.BulletsInMagazine -= 1;
		return true;
	}
	return false;
}

void Gun::Draw()
{
	DxLib::DrawFormatString(200, 200, 0xff0000, "残弾数：%d", _gun.remainingBullets);
	DxLib::DrawFormatString(200, 300, 0xff0000, "マガジン内残弾数：%d", _gun.BulletsInMagazine);
}
