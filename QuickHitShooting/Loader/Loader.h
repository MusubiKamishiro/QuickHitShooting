#pragma once
#include <string>

///ロード基底クラス
class Loader
{
public:
	Loader();
	virtual ~Loader();

	///ロード
	///@param path ファイルのパス
	///@param data データオブジェクトへの参照
	///@retval true 成功
	///@retval false 失敗
	virtual int Load(const std::string& path) = 0;

	///データのアンロード
	///@attention	これで指定されたパスに対応するデータはメモリ上から無くなるので
	///				もう一回使用するときはストレージアクセスが発生するよ
	virtual void UnLoad(const std::string& path) = 0;
};

