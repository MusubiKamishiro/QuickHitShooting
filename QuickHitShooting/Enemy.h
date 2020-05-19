#pragma once
#include "Geometry.h"

class Enemy
{
private:

protected:
	Vector2<int> _pos;
	Rect _rect;

	int _dispTime;
	int _appearTime;

	bool _isAlive;		// true : 生存, false : 死亡
public:
	virtual ~Enemy();

	virtual void Draw() = 0;
	void Update(); 

	// 弾が当たった時の処理
	void HitShot();

	// 敵の削除用
	bool Destroy();

	virtual int GetScore() const = 0;
	Vector2<int> GetPos() const;
	Rect GetRect() const;
};

