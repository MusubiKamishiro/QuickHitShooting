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

	int _alpha;				// 透明度用の値

	const int _alphaMax;	// 透明度の最大値

	// 的IDの矩形色(通常 : 黄色, 特別 : 赤色, 減点 : 青色)
	const int _typeColor[static_cast<int>(TargetID::MAX)] = {
		0xffff00, 0xff2323, 0x2323ff
	};

};

