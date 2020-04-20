#pragma once
#include <memory>
#include <array>
#include "Scene.h"

class TrimString;

class SelectScene : public Scene
{
private:
	void (SelectScene::*_updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	std::unique_ptr<TrimString> _trimString;
public:
	SelectScene();
	~SelectScene();

	void Update(const Peripheral& p);
	void Draw();
};

