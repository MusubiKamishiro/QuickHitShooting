#include "EffekseerLoader.h"
#include "EffekseerForDXLib.h"

EffekseerLoader::EffekseerLoader()
{
}

EffekseerLoader::~EffekseerLoader()
{
}

bool EffekseerLoader::Load(const std::string& path, Data& data)
{
	EffekseerData& effect = dynamic_cast<EffekseerData&>(data);

	if (_table.find(path) == _table.end())
	{
		/// エフェクトの拡大率は仮設定
		effect._handle = LoadEffekseerEffect(path.c_str(), 15.f);
		if (effect._handle != -1)
		{
			/// エフェクトの登録
			_table.emplace(path, effect._handle);
			return true;
		}
	}
	else
	{
		effect._handle = _table[path];
	}
	return false;
}

void EffekseerLoader::UnLoad(const std::string& path)
{
}

bool EffekseerData::IsAvailable()
{
	return false;
}

int EffekseerData::GetHandle()
{
	return _handle;
}
