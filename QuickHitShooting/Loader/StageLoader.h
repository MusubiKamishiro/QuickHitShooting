#pragma once
#include "Loader.h"

class StageLoader :
	public Loader
{
private:
	/// ステージデータに必要なものを書く
public:
	StageLoader();
	~StageLoader();

	int Load(const std::string& path) override;

	void UnLoad(const std::string& path)override;
};

