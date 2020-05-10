#include "SoundLoader.h"
#include <DxLib.h>


SoundLoader::SoundLoader()
{
}


SoundLoader::~SoundLoader()
{
}

bool SoundLoader::Load(const std::string& path, Data& data)
{
	SoundData& sound = dynamic_cast<SoundData&>(data);

	// データが見つからなかったら読み込む
	auto it = _table.find(path.c_str());
	if (it == _table.end())
	{
		sound._handle = DxLib::LoadSoundMem(path.c_str());
		if (sound._handle != -1)
		{
			_table.emplace(path, sound._handle);
			return true;
		}
		return false;
	}
	else
	{
		// 見つかったらハンドルを返す
		sound._handle = _table[path.c_str()];
		return true;
	}
	return false;
}

void SoundLoader::UnLoad(const std::string& path)
{
}

bool SoundData::IsAvailable()
{
	return true;
}

int SoundData::GetHandle() const
{
	return _handle;
}
