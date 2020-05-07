#pragma once
#include "Loader.h"

struct TargetData
{
	unsigned char type;
	unsigned int  dispTime;
	unsigned int appearTime;

	/// 座標も入れるかもしれない
};

class StageLoader :
	public Loader
{
private:
	/// ステージデータに必要なものを書く
	const char _waveEnd;
public:
	StageLoader();
	~StageLoader();

	int Load(const std::string& path) override;

	void UnLoad(const std::string& path)override;
};

