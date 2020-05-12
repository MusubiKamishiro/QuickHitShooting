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
	if (_table.find(path.c_str()) == _table.end())
	{
		FILE* file;
		/// フォルダーで指定したファイルを開く
		fopen_s(&file, path.c_str(), "rb");

		TargetData target;
		std::vector<TargetData> targetData;
		int bytePos   = 0;
		char checkVal = 0;

		fseek(file, bytePos, SEEK_SET);
		while (checkVal != -1)
		{
			/// 読み込むバイト番地を指定する
			fseek(file, bytePos, SEEK_SET);

			{/* 的情報の読み込み */

				/// 的の種別ID
				fread(&target.type, sizeof(target.type), 1, file);
				bytePos += sizeof(target.type);

				/// 的の出現する時間
				fread(&target.dispTime, sizeof(target.dispTime), 1, file);
				bytePos += sizeof(target.dispTime);

				/// 的が出現してから消えるまでの時間
				fread(&target.appearTime, sizeof(target.appearTime), 1, file);
				bytePos += sizeof(target.appearTime);

				/// 的のX座標
				fread(&target.pos.x, sizeof(target.pos.x), 1, file);
				bytePos += sizeof(target.pos.x);

				/// 的のY座標
				fread(&target.pos.y, sizeof(target.pos.y), 1, file);
				bytePos += sizeof(target.pos.y);
			}

			/// 的情報の登録
			targetData.push_back(target);

			/// ウェーブ数の末尾かの確認を行う
			fread(&checkVal, sizeof(checkVal), 1, file);
			if (checkVal == _waveEnd)
			{
				/// 1ウェーブで出現する的情報の登録
				bytePos += sizeof(checkVal);
				_table[path].push_back(targetData);
				targetData.clear();
			}

			/// eofの確認を行うための読み込み
			fread(&checkVal, sizeof(checkVal), 1, file);
		}
		fclose(file);
		stage._stageData = _table[path];
		return false;
	}
	// 見つかったらデータを返す
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
