#pragma once
#include "Loader.h"
#include <map>


class ImageLoader : public Loader
{
private:
	///画像のテーブルマップ
	///@param string ファイルパス
	///@param int グラフィックハンドル
	std::map<std::string, int> _table;

public:
	ImageLoader();
	~ImageLoader();

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

class ImageData : public Data
{
	friend ImageLoader;
private:
	int _handle;	// ハンドル

	///ダミー関数
	bool IsAvailable();

public:
	///読み込んだハンドルの獲得
	///@return グラフィックハンドル
	int GetHandle()const;
};
