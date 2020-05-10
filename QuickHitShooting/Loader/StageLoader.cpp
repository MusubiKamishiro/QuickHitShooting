#include "StageLoader.h"

StageLoader::StageLoader() : _waveEnd(55)
{
}

StageLoader::~StageLoader()
{
}

bool StageLoader::Load(const std::string& path, Data& data)
{
	StageData& stage = dynamic_cast<StageData&>(data);

	// データが見つからなかったら読み込む
	auto it = _table.find(path.c_str());
	if (it == _table.end())
	{
		// 読み込み
		//stage._targetData = ;

		// 読み込み結果が成功したら_tableに追加
		/*if (stage._targetData != nullptr)
		{
			_table.emplace(path, stage._targetData);
			return true;
		}*/
		return false;
	}
	else
	{
		// 見つかったらデータを返す
		stage._targetData = _table[path.c_str()];
		return true;
	}
	return false;
}

void StageLoader::UnLoad(const std::string& path)
{
}

bool StageData::IsAvailable()
{
	return true;
}

TargetData StageData::GetTargetData() const
{
	return _targetData;
}
