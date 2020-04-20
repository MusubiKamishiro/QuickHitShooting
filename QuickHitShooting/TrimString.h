#pragma once
#include <string>
#include "Geometry.h"

///文字列描画のちょうどいい位置を返すクラス
class TrimString
{
private:
	// ゲーム画面のサイズ
	Vector2<int> ssize;
	// フォントサイズ
	int _fontSize;

public:
	TrimString();
	~TrimString();

	// 引数の文字列を画面の中央に揃える座標を返す
	//@param name...使用する文字列
	//@return 文字列の左端のx座標
	int GetStringCenterPosx(const std::string& name)const;
	// 引数の文字列を右詰めにする座標を返す
	//@param name...使用する文字列
	//@param rpos...揃えたい右端の座標
	//@return 文字列の左端のx座標
	int GetStringRightPosx(const std::string& name, const int& rpos)const;
	// 現在のフォントサイズを返す
	int GetFontSize()const;

	// フォントサイズの変更
	void ChangeFontSize(const int& fontSize);
};

