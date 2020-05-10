#pragma once
#include "Loader.h"
#include <map>

struct TargetData
{
	unsigned char type;
	unsigned int  dispTime;
	unsigned int appearTime;

	/// 座標も入れるかもしれない
};

class StageLoader : public Loader
{
private:
	///ステージのテーブルマップ
	///@param string ファイルパス
	///@param TargetData サウンドハンドル
	std::map<std::string, TargetData> _table;
	/// ステージデータに必要なものを書く
	const char _waveEnd;
public:
	StageLoader();
	~StageLoader();

	///ロード
	///@param path ファイルのパス
	///@param data データオブジェクトへの参照
	///@retval true 成功
	///@retval false 失敗
	bool Load(const std::string& path, Data& data) override;

	/// アンロード
	///@param path ファイルのパス
	void UnLoad(const std::string& path)override;
};

class StageData : public Data
{
	friend StageLoader;
private:
	TargetData _targetData;

	///ダミー関数
	bool IsAvailable();

public:
	TargetData GetTargetData()const;
};