#pragma once
#include "TargetState.h"
class TargetPosition :
	public TargetState
{
public:
	TargetPosition();
	~TargetPosition();

	void Update(int& wCnt, int& tCnt, 
				const unique_input& input, vec2_target& stageData) override;
private:
	void Draw(const int& wCnt, const int& tCnt,
			  const vec2_target stageData) override;
	void DataConfig(const int& wCnt, const int& tCnt, 
					const unique_input& input, vec2_target& stageData) override;
};

