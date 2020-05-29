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
	// データが見つからなかったら読み込む
	if (_table.find(path.c_str()) == _table.end())
	{
		/// フォルダーで指定したファイルを開く
		if (fopen_s(&file, path.c_str(), "rb") == 0)
		{
			StageInfo stageInfo;
			/// スコアデータの読み込み
			for (unsigned int i = 0; i < stageInfo.scores.size(); ++i)
			{
				/// 文字列の初期化
				stageInfo.names[i] = "AAA";
				fread(&stageInfo.scores[i], sizeof(int), 1, file);
				fread((char*)stageInfo.names[i].c_str(), (sizeof(char) * 3), 1, file);
			}

			/// ステージデータの読み込み時に使用するもの
			int waveCnt, targetCnt;
			TargetData target;
			vec_target targetData;

			/// ウェーブ数の読み込み
			fread(&waveCnt, sizeof(int), 1, file);

			for (int w = 0; w < waveCnt; ++w)
			{
				/// 的数の読み込み
				fread(&targetCnt, sizeof(int), 1, file);
				targetData.resize(targetCnt);

				/// 的情報の読み込み
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

				/// 的情報を開放する
				targetData.clear();
				std::vector<TargetData>().swap(targetData);
			}
			fclose(file);

			/// ステージデータの登録
			stageInfo.stageName = path;
			_table[path]		= stageInfo;
			
			/// ステージデータを開放する
			stageInfo.targetData.clear();
			std::vector<vec_target>().swap(stageInfo.targetData);
		}
	}
	else
	{
		if (fopen_s(&file, path.c_str(), "rb") == 0)
		{
			/// スコアデータの読み込み
			for (unsigned int i = 0; i < _table[path].scores.size(); ++i)
			{
				/// 文字列の初期化
				_table[path].names[i] = "AAA";
				fread(&_table[path].scores[i], sizeof(int), 1, file);
				fread((char*)_table[path].names[i].c_str(), (sizeof(char) * 3), 1, file);
			}
		}
		fclose(file);
	}
	/// パスが見つかったらデータを返す。
	stage._stageData = _table[path];

	return true;
}

void StageLoader::UnLoad(const std::string& path)
{
	/// 登録物の削除
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
