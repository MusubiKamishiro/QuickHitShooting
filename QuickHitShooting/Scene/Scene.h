#pragma once
#include <memory>
#include "../Geometry.h"

class Peripheral;

// (純粋仮想クラス)
class Scene
{
protected:
	// 描画ブレンドモードのパラメータ(0~255)
	int _pal;
	// 画面のサイズ
	Vector2<int> _scrSize;
	// シーン開始からの経過時間
	int _sceneTime;

public:
	Scene();
	virtual ~Scene();

	// シーンの更新を行う
	virtual void Update(const Peripheral& p) = 0;
	// シーンの描画
	virtual void Draw() = 0;
	
	// フェードインの処理を書く
	virtual void FadeinUpdate(const Peripheral& p) = 0;
	// フェードアウトの処理を書く
	virtual void FadeoutUpdate(const Peripheral& p) = 0;
	// 待機中の処理を書く
	virtual void WaitUpdate(const Peripheral& p) = 0;

	// シーンの進行時間を計っている
	void AddSceneTime();
};

