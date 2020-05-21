#include "Gun.h"
#include <DxLib.h>
#include "Game.h"
#include "TrimString.h"

Gun::Gun(const GunStatus& gunState)
{
	_gun = gunState;
	_gun.remainingBullets = gunState.maxBullets;
	_gun.BulletsInMagazine = gunState.maxBulletsInMagazine;
	_trimString.reset(new TrimString());
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
	Vector2<int> ssize = Game::Instance().GetScreenSize();
	_trimString->ChangeFontSize(30);

	DxLib::DrawFormatString(750, ssize.y - _trimString->GetFontSize(), 0xff0000, "残弾数：%d", _gun.remainingBullets);
	DxLib::DrawFormatString(950, ssize.y - _trimString->GetFontSize(), 0xff0000, "マガジン内残弾数：%d", _gun.BulletsInMagazine);
}
