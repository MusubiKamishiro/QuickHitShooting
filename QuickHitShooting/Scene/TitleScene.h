#pragma once
#include "Scene.h"
#include <memory>
#include "../Geometry.h"

class TrimString;

class TitleScene : public Scene
{
private:
	void (TitleScene::*_updater)(const Peripheral& p);
	void (TitleScene::* _drawer)();

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	void StartDraw();
	void SelectPlayDraw();
	
	std::unique_ptr<TrimString> _trimString;

public:
	TitleScene();
	~TitleScene();

	void Update(const Peripheral& p);
	void Draw();
};

