#include "SoundLoader.h"
#include <DxLib.h>


SoundLoader::SoundLoader()
{
}


SoundLoader::~SoundLoader()
{
}

int SoundLoader::Load(const std::string& path)
{
	int sound = 0;

	// データが見つからなかったら読み込む
	auto it = table.find(path.c_str());
	if (it == table.end())
	{
		sound = DxLib::LoadSoundMem(path.c_str());
		if (sound != -1)
		{
			table.emplace(path, sound);
		}
		return sound;
	}
	else
	{
		// 見つかったらハンドルを返す
		return table[path.c_str()];
	}
	return false;
}

void SoundLoader::UnLoad(const std::string& path)
{
}
