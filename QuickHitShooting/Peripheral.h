#pragma once
#include <vector>
#include <array>
#include <map>
#include <string>
#include "Geometry.h"

// 周辺機器(マウス)の入力を制御
class Peripheral
{
private:
	int mouseState;		// マウスの入力情報
	int oldMouseState;	// 1フレーム前のマウスの入力情報

	int pointImg;		// マウスポインターの画像
	Vector2<int> mousePos;	// マウスの座標

public:
	Peripheral();
	~Peripheral();

	///入力情報の更新(毎フレーム)
	void Update();

	///現在の押下状態の検出
	///@param cmd	調べたいコマンド
	///@retval true	押してる, false 押してない
	bool IsPressing(const int& cmd)const;

	///現在のトリガー状態(押した瞬間)の検出
	///@param cmd	調べたいコマンド
	///@retval true	押した, false 押してない
	bool IsTrigger(const int& cmd)const;

	///マウスの座標を返す
	///@return マウスの座標
	Vector2<int> GetMousePos()const;

	void Draw();
};

