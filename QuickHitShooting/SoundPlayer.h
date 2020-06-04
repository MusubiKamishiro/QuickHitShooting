#pragma once
#include <map>
#include <string>


class SoundPlayer
{
private:
	///サウンドのテーブルマップ
	///@param string サウンド名
	///@param int サウンドハンドル
	std::map<std::string, int> _table;

public:
	SoundPlayer();
	~SoundPlayer();

	///使用するサウンドの追加
	///@param soundname サウンド名
	///@param handle	サウンドハンドル
	///@param volume	ボリューム(最大100%)
	///@retval true 成功
	///@retval false 失敗
	bool AddSound(const std::string& soundname, const int& handle, const int& volume = 100);

	///サウンドを再生させる
	///@param soundname サウンド名
	///@param loop		ループフラグ(trueでループ再生)
	void PlaySound(const std::string& soundname, const bool& loop = false);

	///サウンドを停止させる
	///@param soundname サウンド名
	void StopSound(const std::string& soundname);
};

