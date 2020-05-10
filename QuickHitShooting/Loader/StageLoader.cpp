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

		/// ファイル読み込みを行う
		if (fopen_s(&file, path.c_str(), "rb") != 0)
		{
			/// ファイル読み込み失敗
			MessageBox(GetMainWindowHandle(),
				"Failed to Loading stage",
				"ステージ読み込みに失敗しました。",
				MB_OK);
			return -1;
		}

		/// データ読み込みに必要なもの
		TargetData target;
		std::vector<TargetData> targetData;
		int bytePos   = 0;
		char checkVal = 0;

		/// バイナリの先頭ポインタを指定する
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
				bytePos += sizeof(target.dispTime);

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
	}

	return 0;
}

void StageLoader::UnLoad(const std::string& path)
{
	
}
