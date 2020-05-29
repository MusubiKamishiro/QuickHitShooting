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
	MenuData() : rect(0, 0, 0, 0), useRect(0, 0, 0, 0), img(-1), flag(false) {};
	MenuData(int img, Rect rect, Rect userect = Rect()) : img(img), rect(rect), useRect(userect), flag(false) {};
	int img;		// 画像
	Rect rect;		// 座標と幅高
	Rect useRect;	// 画像内の使用する範囲
	bool flag;		// カーソルがrect内に入ってるとtrue返す
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

	int img;
	int size;

public:
	Menu();
	~Menu();

	///メニューリストに追加
	///@param name	メニュー名
	///@param ltPos	画像を表示する左上座標
	///@param rbPos	画像を表示する右下座標
	///@param size	画像の幅高
	///@param img	画像ハンドル
	void AddMenuList(const std::string& name, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const int& img);
	///@param imgRect	画像内の使用する範囲
	void AddMenuList(const std::string& name, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const Rect& imgRect, const int& img);

	///引数のメニューをクリックしたかを確認する
	///@param name	確認したいメニュー名
	///@retval true	押した, false 押してない
	bool CheckClick(const std::string& name, const Peripheral& p);

	void Update(const Peripheral &p);
	
	///メニューの描画
	///追加されたもの全て描画する
	void Draw();
};

