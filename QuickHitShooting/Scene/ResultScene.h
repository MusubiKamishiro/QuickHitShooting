#pragma once
#include "Scene.h"
#include <memory>
#include <array>
#include "../Geometry.h"
#include "../Gun.h"

class TrimString;

//���U���g���
struct ResultData
{
	int score;					// �l���X�R�A
	std::array<int, 3> ranking;	// �����L���O
	int hitRate;				// ������
	GunStatus gunStatus;		// �e��
};


class ResultScene : public Scene
{
private:
	void (ResultScene::*_updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	ResultData _resultData;


	std::unique_ptr<TrimString> _trimString;

public:
	ResultScene(const int& score);
	~ResultScene();

	void Update(const Peripheral& p);
	void Draw();
};

