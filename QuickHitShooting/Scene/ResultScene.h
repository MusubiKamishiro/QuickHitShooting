#pragma once
#include "Scene.h"
#include <memory>
#include <vector>
#include <array>
#include <string>
#include "../Geometry.h"
#include "../Gun.h"

class TrimString;
class Menu;
class Keyboard;

// ���U���g���
struct ResultData
{
	std::array<std::pair<std::string, int>, 3> ranking;	// �����L���O
	int score;					// �l���X�R�A
	float hitRate;				// ������
	GunStatus gunStatus;		// �e��
	std::string name;
};

// ���l�̃f�[�^
struct NumData
{
	NumData() : num(0.0f), nowDigit(0), digit(0), digitNums() {};

	float num;					// ���l
	unsigned int nowDigit;		// �J�E���g���̌���
	unsigned int digit;			// �f�[�^�̌���
	std::vector<int> digitNums;	// �����Ƃ̐��l
};

class ResultScene : public Scene
{
private:
	void (ResultScene::*_updater)(const Peripheral& p);
	
	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void ScoreUpdate(const Peripheral& p);
	void HitRateUpdate(const Peripheral& p);
	void RankinUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	bool SaveRanking();

	ResultData _resultData;

	std::unique_ptr<TrimString> _trimString;
	std::shared_ptr<Menu> _menu;
	std::shared_ptr<Keyboard> _keyboard;

	int _resultBg;
	int _resultBd;

	int _time;

	NumData _score;		// ���_
	NumData _hitRate;	// ������

	///�������m�F���ANamData�ɒl�𔽉f������
	///@param numDara	���l�f�[�^
	///@param num		�������m�F���������l
	///@param maxDigit	�ő包��
	void CheckDigit(NumData& numData, const int& num, const int& maxDigit);

	///�\���̍ۂɐ��l�������_���ɂ���ۂɎg�p
	///@param maxDigit	�ő包��
	///@param numDara	���l�f�[�^
	float RandomCountUp(const unsigned int& maxDigit, const NumData& numData);

public:
	ResultScene(const ResultData& resultData);
	~ResultScene();

	void Update(const Peripheral& p);
	void Draw();
};

