#pragma once
#include <memory>
#include <array>
#include <vector>
#include "Scene.h"
#include "../Gun.h"

class TrimString;
class Menu;

class SelectScene : public Scene
{
private:
	void (SelectScene::*_updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	std::unique_ptr<TrimString> _trimString;
	std::shared_ptr<Menu> _menu;
	GunStatus _gunState;

	std::vector<GunStatus> _gunStatus;
	
	///銃のメニューの追加
	///@param GunState	銃のステータス
	///@param pos		画像の中心座標
	///@param size		画像のサイズ
	///@param img		画像のハンドル
	void AddGunMenu(const GunStatus& gunstate, const Vector2<int>& pos, const Size& size, const int& img);

public:
	SelectScene();
	~SelectScene();

	void Update(const Peripheral& p);
	void Draw();
};

