#pragma once
#include "Scene.h"
#include <memory>
#include "../Geometry.h"

class TrimString;

class ResultScene : public Scene
{
private:
	void (ResultScene::*_updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	Vector2<int> _center;

	int _bgm;

	int _rpImg;
	int _lpImg;
	int _frameImg;

	std::unique_ptr<TrimString> _trimString;

public:
	ResultScene(const int& score);
	~ResultScene();

	void Update(const Peripheral& p);
	void Draw();
};

