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
	

	int _alpha;				// �����x�p�̒l

	const int _alphaMax;	// �����x�̍ő�l

	// �IID�̋�`�F(�ʏ� : ���F, ���� : �ԐF, ���_ : �F)
	const int _typeColor[static_cast<int>(TargetID::MAX)] = {
		0xffff00, 0xff2323, 0x2323ff
	};

};

