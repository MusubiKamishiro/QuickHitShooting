#pragma once
#include "Loader.h"
#include "../Geometry.h"
#include <map>
#include <vector>
#include <array>

struct TargetData
{
	unsigned char type;
	unsigned int  dispTime;
	unsigned int  appearTime;
	Vector2<int>  pos;
};

using vec2_target = std::vector<std::vector<TargetData>>;

struct StageInfo
{
	std::array<int, 3> scores;
	std::array<std::string, 3> names;
	vec2_target targetData;
};

class StageLoader : public Loader
{
private:
	///ステージのテーブルマップ
	///@param string ファイルパス
	///@param TargetData サウンドハンドル
	std::map<std::string, StageInfo> _table;
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
	void UnLoad(const std::string& path) override;
};

class StageData : public Data
{
	friend StageLoader;
private:
	StageInfo _stageData;

	///ダミー関数
	bool IsAvailable();

public:
	StageInfo GetStageData()const;
};