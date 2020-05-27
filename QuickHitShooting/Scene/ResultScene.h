#pragma once
#include "Scene.h"
#include <memory>
#include <vector>
#include <array>
#include <string>
#include "../Geometry.h"
#include "../Gun.h"

class TrimString;

//���U���g���
struct ResultData
{
	std::array<std::pair<std::string, int>, 3> ranking;	// �����L���O
	int score;					// �l���X�R�A
	float hitRate;				// ������
	GunStatus gunStatus;		// �e��
};


class ResultScene : public Scene
{
private:
	void (ResultScene::*_updater)(const Peripheral& p);
	
	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void ScoreUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	ResultData _resultData;

	std::unique_ptr<TrimString> _trimString;

	int _resultScore;
	int _time;
	unsigned int _nowDigit;		// �J�E���g���̌�
	unsigned int _scoreDigit;	// �X�R�A�̌���
	std::vector<int> _num;

	void CheckScore(const int& inscore);

public:
	ResultScene(const ResultData& resultData);
	~ResultScene();

	void Update(const Peripheral& p);
	void Draw();
};

