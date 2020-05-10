#pragma once
#include "Loader.h"
#include <map>


class SoundLoader : public Loader
{
private:
	///サウンドのテーブルマップ
	///@param string ファイルパス
	///@param int サウンドハンドル
	std::map<std::string, int> _table;

public:
	SoundLoader();
	~SoundLoader();

	///ロード
	///@param path ファイルのパス
	///@param data データオブジェクトへの参照
	///@retval true 成功
	///@retval false 失敗
	bool Load(const std::string& path, Data& data)override final;

	/// アンロード
	///@param path ファイルのパス
	void UnLoad(const std::string& path)override final;
};

class SoundData : public Data
{
	friend SoundLoader;
private:
	int _handle;	// ハンドル

	///ダミー関数
	bool IsAvailable();

public:
	///読み込んだハンドルの獲得
	///@return サウンドハンドル
	int GetHandle()const;
};