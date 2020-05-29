#include "StageLoader.h"

StageLoader::StageLoader()
{
}

StageLoader::~StageLoader()
{
}

bool StageLoader::Load(const std::string& path, Data& data)
{
	StageData& stage = dynamic_cast<StageData&>(data);

	FILE* file;
	// �f�[�^��������Ȃ�������ǂݍ���
	if (_table.find(path.c_str()) == _table.end())
	{
		/// �t�H���_�[�Ŏw�肵���t�@�C�����J��
		if (fopen_s(&file, path.c_str(), "rb") == 0)
		{
			StageInfo stageInfo;
			/// �X�R�A�f�[�^�̓ǂݍ���
			for (unsigned int i = 0; i < stageInfo.scores.size(); ++i)
			{
				/// ������̏�����
				stageInfo.names[i] = "AAA";
				fread(&stageInfo.scores[i], sizeof(int), 1, file);
				fread((char*)stageInfo.names[i].c_str(), (sizeof(char) * 3), 1, file);
			}

			/// �X�e�[�W�f�[�^�̓ǂݍ��ݎ��Ɏg�p�������
			int waveCnt, targetCnt;
			TargetData target;
			vec_target targetData;

			/// �E�F�[�u���̓ǂݍ���
			fread(&waveCnt, sizeof(int), 1, file);

			for (int w = 0; w < waveCnt; ++w)
			{
				/// �I���̓ǂݍ���
				fread(&targetCnt, sizeof(int), 1, file);
				targetData.resize(targetCnt);

				/// �I���̓ǂݍ���
				for (int t = 0; t < targetCnt; ++t)
				{
					fread(&target.type,			sizeof(unsigned char), 1, file);
					fread(&target.dispTime,		sizeof(unsigned int), 1, file);
					fread(&target.appearTime,	sizeof(unsigned int), 1, file);
					fread(&target.pos.x,		sizeof(int), 1, file);
					fread(&target.pos.y,		sizeof(int), 1, file);

					targetData[t] = target;
				}
				stageInfo.targetData.push_back(targetData);

				/// �I�����J������
				targetData.clear();
				std::vector<TargetData>().swap(targetData);
			}
			fclose(file);

			/// �X�e�[�W�f�[�^�̓o�^
			stageInfo.stageName = path;
			_table[path]		= stageInfo;
			
			/// �X�e�[�W�f�[�^���J������
			stageInfo.targetData.clear();
			std::vector<vec_target>().swap(stageInfo.targetData);
		}
	}
	else
	{
		if (fopen_s(&file, path.c_str(), "rb") == 0)
		{
			/// �X�R�A�f�[�^�̓ǂݍ���
			for (unsigned int i = 0; i < _table[path].scores.size(); ++i)
			{
				/// ������̏�����
				_table[path].names[i] = "AAA";
				fread(&_table[path].scores[i], sizeof(int), 1, file);
				fread((char*)_table[path].names[i].c_str(), (sizeof(char) * 3), 1, file);
			}
		}
		fclose(file);
	}
	/// �p�X������������f�[�^��Ԃ��B
	stage._stageData = _table[path];

	return true;
}

void StageLoader::UnLoad(const std::string& path)
{
	/// �o�^���̍폜
	_table.erase(path);
}

bool StageData::IsAvailable()
{
	return true;
}

StageInfo StageData::GetStageData() const
{
	return _stageData;
}
