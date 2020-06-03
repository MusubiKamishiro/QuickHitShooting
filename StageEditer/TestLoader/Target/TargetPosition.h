#pragma once
#include "TargetState.h"
class TargetPosition :
	public TargetState
{
public:
	TargetPosition();
	~TargetPosition();

	void Update(int& wCnt, int& tCnt, 
				const unique_input& input, std::vector<vec_target>& stageData) override;

	void Draw(const int& wCnt, const int& tCnt,
			  const std::vector<vec_target> stageData) override;
protected:
	void DataConfig(const int& wCnt, const int& tCnt,
				    const unique_input& input, std::vector<vec_target>& stageData) override;
private:
	bool CheckTargetRange(const Vector2<int>& pos) const;	// 的を配置できる範囲であるかの判定用
	int _alpha;					// 透明度用の値

	const int _alphaMax;		// 透明度の最大値
};

