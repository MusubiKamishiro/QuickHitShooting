#include "SoundPlayer.h"
#include <DxLib.h>
#undef PlaySound;

SoundPlayer::SoundPlayer()
{
}

SoundPlayer::~SoundPlayer()
{
}

bool SoundPlayer::AddSound(const std::string& soundname, const int& handle, const int& volume = 100)
{
	// 万が一ハンドルが空だった場合
	if (handle == -1)
	{
		return false;
	}

	// データが見つからなかったら読み込む
	auto it = _table.find(soundname.c_str());
	if (it == _table.end())
	{
		DxLib::ChangeVolumeSoundMem(255 * volume / 100, handle);
		_table.emplace(soundname, handle);
		return true;
	}

	return false;
}

void SoundPlayer::PlaySound(const std::string& soundname, const bool& loop = false)
{
	if (!DxLib::CheckSoundMem(_table[soundname.c_str()]))
	{
		if (!loop)
		{
			DxLib::PlaySoundMem(_table[soundname.c_str()], DX_PLAYTYPE_BACK);
		}
		else
		{
			DxLib::PlaySoundMem(_table[soundname.c_str()], DX_PLAYTYPE_LOOP);
		}
	}
}

void SoundPlayer::StopSound(const std::string& soundname)
{
	DxLib::StopSoundMem(_table[soundname.c_str()]);
}
