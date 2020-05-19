#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "Geometry.h"

class Peripheral;
class CollisionDetector;

// メニューに必要な情報
struct MenuData
{
	MenuData() : rect(0, 0, 0, 0), img(-1) {};
	MenuData(Rect rect, int img) : rect(rect), img(img) {};
	Rect rect;		// 座標と幅高
	int img;		// 画像
};

class Menu
{
private:
	///メニューテーブル
	///@param string ボタン名
	///@param int メニューデータ
	std::map<std::string, MenuData> _menuTable;

	// 当たり判定
	std::shared_ptr<CollisionDetector> _cd;

public:
	Menu();
	~Menu();

	///メニューリストに追加
	///@param name	メニュー名
	///@param pos	画像の表示座標(中心)
	///@param size	画像の幅高
	///@param img	画像ハンドル
	void AddMenuList(const std::string& name, const Vector2<int>& pos, const Size& size, const int& img);

	///引数のメニューをクリックしたかを確認する
	///@param name	確認したいメニュー名
	///@retval true	押した, false 押してない
	bool CheckCrick(const std::string& name, const Peripheral& p);

	void Update(const Peripheral &p);
	
	///メニューの描画
	///追加されたもの全て描画する
	void Draw();
};

