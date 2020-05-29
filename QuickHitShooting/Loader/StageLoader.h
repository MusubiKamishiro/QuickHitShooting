#pragma once
#include "Loader.h"
#include "../Geometry.h"
#include <map>
#include <vector>
#include <array>

// 的情報
struct TargetData
{
	unsigned char type;			// ID
	unsigned int  dispTime;		// 表示時間
	unsigned int  appearTime;	// 表示してから消えるまでの時間
	Vector2<int>  pos;			// 座標
};
// 的情報の可変長配列
using vec_target = std::vector<TargetData>;

struct StageInfo
{
	std::array<int, 3> scores;
	std::array<std::string, 3> names;
	std::vector<vec_target> targetData;

	/// ステージネーム取得用
	std::string stageName;
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
	StageInfo GetStageData() const;
};