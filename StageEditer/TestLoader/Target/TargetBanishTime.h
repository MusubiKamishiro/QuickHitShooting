#pragma once
#include "TargetState.h"

class TargetBanishTime : public TargetState
{
public:
	TargetBanishTime();
	~TargetBanishTime();

	void Update(int& waveCnt, int& targetCnt,
				const unique_input& input,std::vector<vec_target>& stageData) override;

	void Draw(const int& wCnt, const int& tCnt,
			  const std::vector<vec_target> stageData) override;
protected:
	void DataConfig(const int& wCnt, const int& tCnt,
					const unique_input& input, std::vector<vec_target>& stageData) override;
};

