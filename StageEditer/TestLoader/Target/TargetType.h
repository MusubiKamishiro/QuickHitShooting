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

	/// 的のID用テキスト(ID用の画像はゲームの的が確定してから差し替える)
	const std::string _idText[static_cast<int>(TargetID::MAX)] = {
		"通常", "特別", "減点"
	};
};

