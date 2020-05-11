#pragma once
#include "TargetState.h"
class TargetAppearTime :
	public TargetState
{
public:
	TargetAppearTime();
	~TargetAppearTime();

	void Update(int& waveCnt, int& targetCnt,
				const unique_input& input,vec2_target& stageData) override;
private:
	void DataConfig(const int& wCnt, const int& tCnt,
					const unique_input& input, vec2_target& stageData) override;
};

