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

	// �f�[�^��������Ȃ�������ǂݍ���
	auto it = _table.find(path.c_str());
	if (it == _table.end())
	{
		// �ǂݍ���
		//stage._targetData = ;

		// �ǂݍ��݌��ʂ�����������_table�ɒǉ�
		/*if (stage._targetData != nullptr)
		{
			_table.emplace(path, stage._targetData);
			return true;
		}*/
		return false;
	}
	else
	{
		// ����������f�[�^��Ԃ�
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
