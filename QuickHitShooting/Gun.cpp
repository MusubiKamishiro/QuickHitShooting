#include "Gun.h"
#include <DxLib.h>
#include "Game.h"
#include "TrimString.h"

#include "Game.h"
#include "Loader/FileSystem.h"
#include "Loader/SoundLoader.h"
#include "SoundPlayer.h"

Gun::Gun(const GunStatus& gunState)
{
	_gun = gunState;
	_gun.remainingBullets = gunState.maxBullets;
	_gun.BulletsInMagazine = gunState.maxBulletsInMagazine;
	_trimString.reset(new TrimString());

	SoundData data;
	Game::Instance().GetFileSystem()->Load("sound/se/handgun-out-bullets.mp3", data);
	Game::Instance().GetSoundPlayer()->AddSound("outOfAmmo", data.GetHandle(), 80);
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
		Game::Instance().GetSoundPlayer()->PlaySound("shot");
		_gun.BulletsInMagazine -= 1;
		return true;
	}

	Game::Instance().GetSoundPlayer()->PlaySound("outOfAmmo");
	return false;
}

void Gun::Draw()
{
	Vector2<int> ssize = Game::Instance().GetScreenSize();
	_trimString->ChangeFontSize(70);

	DxLib::DrawFormatString(650, ssize.y - _trimString->GetFontSize() - 8, 0x000000, "残弾数：%d", _gun.remainingBullets);
	DxLib::DrawFormatString(950, ssize.y - _trimString->GetFontSize() - 8, 0x000000, "マガジン内残弾数：%d", _gun.BulletsInMagazine);
}
