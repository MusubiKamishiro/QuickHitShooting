#pragma once
#include "Geometry.h"

class Enemy
{
private:
	Vector2<int> pos;
	Rect rect;

public:
	Enemy();
	~Enemy();

	void Draw();

	Vector2<int> GetPos()const;
	Rect GetRect()const;
};

