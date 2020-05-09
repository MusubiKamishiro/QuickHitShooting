#pragma once
#include <vector>
#include <map>
#include "Loader.h"
#include "../Geometry.h"

struct TargetData
{
	unsigned char type;
	unsigned int  dispTime;
	unsigned int  appearTime;
	Vector2<int>  pos;
};

using vec2_targetData = std::vector<std::vector<TargetData>>;

class StageLoader :
	public Loader
{
private:

	std::map<std::string, vec2_targetData> _table;

	/// バイナリ上でウェーブのエンドポイントを示すもの
	const char _waveEnd;
public:
	StageLoader();
	~StageLoader();

	int Load(const std::string& path) override;

	void UnLoad(const std::string& path)override;
};

