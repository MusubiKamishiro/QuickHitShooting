#pragma once

#include <string>

#include "../Editer.h"
#include "../Input.h"

class TargetState
{
public:
	virtual void Update(int& wCnt, int& tCnt, 
						const unique_input& input, std::vector<vec_target>& stageData) = 0;

	virtual void Draw(const int& wCnt, const int& tCnt,
					  const std::vector<vec_target> stageData) = 0;

protected:
	void ChangeTarget(int& tCnt, const int& tCntMax, const unique_input& input);
	void ChangeWave(int& wCnt, const int& wCntMax, const unique_input& input);

	virtual void DataConfig(const int& wCnt, const int& tCnt,
						    const unique_input& input, std::vector<vec_target>& stageData) = 0;

	Vector2<int> _drawPos, _strSize;
	std::string _text;
	const int _boxSize = 75;
};

