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
	if (_table.find(path.c_str()) == _table.end())
	{
		FILE* file;
		/// �t�H���_�[�Ŏw�肵���t�@�C�����J��
		fopen_s(&file, path.c_str(), "rb");

		TargetData target;
		std::vector<TargetData> targetData;
		int bytePos   = 0;
		char checkVal = 0;

		fseek(file, bytePos, SEEK_SET);
		while (checkVal != -1)
		{
			/// �ǂݍ��ރo�C�g�Ԓn���w�肷��
			fseek(file, bytePos, SEEK_SET);

			{/* �I���̓ǂݍ��� */

				/// �I�̎��ID
				fread(&target.type, sizeof(target.type), 1, file);
				bytePos += sizeof(target.type);

				/// �I�̏o�����鎞��
				fread(&target.dispTime, sizeof(target.dispTime), 1, file);
				bytePos += sizeof(target.dispTime);

				/// �I���o�����Ă��������܂ł̎���
				fread(&target.appearTime, sizeof(target.appearTime), 1, file);
				bytePos += sizeof(target.appearTime);

				/// �I��X���W
				fread(&target.pos.x, sizeof(target.pos.x), 1, file);
				bytePos += sizeof(target.pos.x);

				/// �I��Y���W
				fread(&target.pos.y, sizeof(target.pos.y), 1, file);
				bytePos += sizeof(target.pos.y);
			}

			/// �I���̓o�^
			targetData.push_back(target);

			/// �E�F�[�u���̖������̊m�F���s��
			fread(&checkVal, sizeof(checkVal), 1, file);
			if (checkVal == _waveEnd)
			{
				/// 1�E�F�[�u�ŏo������I���̓o�^
				bytePos += sizeof(checkVal);
				_table[path].push_back(targetData);
				targetData.clear();
			}

			/// eof�̊m�F���s�����߂̓ǂݍ���
			fread(&checkVal, sizeof(checkVal), 1, file);
		}
		fclose(file);
		stage._stageData = _table[path];
		return false;
	}
	// ����������f�[�^��Ԃ�
	stage._stageData = _table[path];

	return true;
}

void StageLoader::UnLoad(const std::string& path)
{
}

bool StageData::IsAvailable()
{
	return true;
}

vec2_target StageData::GetStageData() const
{
	return _stageData;
}
