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
	// 設定するウェーブの変更
	void ChangeWave(int& wCnt, const int& wCntMax, const unique_input& input);

	// 設定する的の変更
	void ChangeTarget(int& tCnt, const int& tCntMax, const unique_input& input);

	// ステージデータの設定
	virtual void DataConfig(const int& wCnt, const int& tCnt,
						    const unique_input& input, std::vector<vec_target>& stageData) = 0;

	Vector2<int> _drawPos, _strSize;
	std::string _text;
private:


	const int _boxSize = 50;
};

