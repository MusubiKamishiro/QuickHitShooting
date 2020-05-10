#include <windows.h>
#include <DxLib.h>
#include "StageLoader.h"
StageLoader::StageLoader() : _waveEnd(55)
{
}

StageLoader::~StageLoader()
{
}

int StageLoader::Load(const std::string& path)
{
	if (_table.find(path.c_str()) == _table.end())
	{
		FILE* file;

		/// �t�@�C���ǂݍ��݂��s��
		if (fopen_s(&file, path.c_str(), "rb") != 0)
		{
			/// �t�@�C���ǂݍ��ݎ��s
			MessageBox(GetMainWindowHandle(),
				"Failed to Loading stage",
				"�X�e�[�W�ǂݍ��݂Ɏ��s���܂����B",
				MB_OK);
			return -1;
		}

		/// �f�[�^�ǂݍ��݂ɕK�v�Ȃ���
		TargetData target;
		std::vector<TargetData> targetData;
		int bytePos   = 0;
		char checkVal = 0;

		/// �o�C�i���̐擪�|�C���^���w�肷��
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
				bytePos += sizeof(target.dispTime);

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
	}

	return 0;
}

void StageLoader::UnLoad(const std::string& path)
{
	
}
