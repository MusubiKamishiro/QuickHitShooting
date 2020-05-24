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

	// �f�[�^��������Ȃ�������ǂݍ���
	if (_table.find(path.c_str()) == _table.end())
	{
		// �Ƃ肠�����f�[�^�̏��������s��
		FILE* file;
		/// �t�H���_�[�Ŏw�肵���t�@�C�����J��
		if (fopen_s(&file, path.c_str(), "rb") == 0)
		{
			StageInfo stageInfo;
			/// �X�R�A�f�[�^�̓ǂݍ���
			for (int i = 0; i < stageInfo.scores.size(); ++i)
			{
				fread(&stageInfo.scores[i], sizeof(int), 1, file);
				fread((char*)stageInfo.names[i].c_str(), (sizeof(char) * 3), 1, file);
			}

			TargetData target;
			std::vector<TargetData> targetData;

			int waveCnt, targetCnt;
			/// �E�F�[�u���̓ǂݍ���
			fread(&waveCnt, sizeof(int), 1, file);

			for (int w = 0; w < waveCnt; ++w)
			{
				fread(&targetCnt, sizeof(int), 1, file);
				targetData.resize(targetCnt);
				for (int t = 0; t < targetCnt; ++t)
				{
					fread(&target.type,			sizeof(unsigned char), 1, file);
					fread(&target.dispTime,		sizeof(unsigned int), 1, file);
					fread(&target.appearTime,	sizeof(unsigned int), 1, file);
					fread(&target.pos.x, sizeof(int), 1, file);
					fread(&target.pos.y, sizeof(int), 1, file);

					targetData[t] = target;
				}
				stageInfo.targetData.push_back(targetData);
				targetData.clear();
				std::vector<TargetData>().swap(targetData);
			}
			fclose(file);
			_table[path] = stageInfo;
			stage._stageData = _table[path];
			
			stageInfo.targetData.clear();
			vec2_target().swap(stageInfo.targetData);
		}
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

StageInfo StageData::GetStageData() const
{
	return _stageData;
}
