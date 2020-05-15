#pragma once

#include <string>

#include "../Stage.h"
#include "../Input.h"

class TargetState
{
public:
	virtual void Update(int& wCnt, int& tCnt, 
						const unique_input& input, vec2_target& stageData) = 0;

protected:
	// 設定する的の変更
	void ChangeTarget(int& tCnt, const int& tCntMax, const unique_input& input);

	// 設定するウェーブの変更
	void ChangeWave(int& wCnt, const int& wCntMax, const unique_input& input);

	void DebugDraw(const int& wCnt, const int& tCnt, vec2_target stageData);
private:
	
	virtual void Draw(const int& wCnt, const int& tCnt,
					  const vec2_target stageData) = 0;

	// ステージデータの設定
	virtual void DataConfig(const int& wCnt, const int& tCnt,
							const unique_input& input, vec2_target& stageData) = 0;

	const int _boxSize = 50;
};

