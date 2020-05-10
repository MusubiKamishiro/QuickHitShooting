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

	// �f�[�^��������Ȃ�������ǂݍ���
	auto it = table.find(path.c_str());
	if (it == table.end())
	{
		sound.handle = DxLib::LoadSoundMem(path.c_str());
		if (sound.handle != -1)
		{
			table.emplace(path, sound.handle);
			return true;
		}
		return false;
	}
	else
	{
		// ����������n���h����Ԃ�
		sound.handle = table[path.c_str()];
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
	return handle;
}
