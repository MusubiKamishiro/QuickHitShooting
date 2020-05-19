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

	// 仮の敵削除用関数
	bool Destroy();

	virtual int GetPoint() const = 0;
	Vector2<int> GetPos() const;
	Rect GetRect() const;
};

