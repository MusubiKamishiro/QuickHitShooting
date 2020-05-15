#pragma once

#include "TargetState.h"

class TargetType :
	public TargetState
{
public:
	TargetType();
	~TargetType();
	
	void Update(int& wCnt, int& tCnt, 
				const unique_input& input, vec2_target& stageData) override;
private:
	void Draw(const int& wCnt, const int& tCnt,
			  const vec2_target stageData) override;
	void DataConfig(const int& wCnt, const int& tCnt, const unique_input& input,
					vec2_target& stageData) override;
};

