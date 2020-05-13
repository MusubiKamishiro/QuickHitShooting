#pragma once
#include "Geometry.h"

class Enemy
{
private:

protected:
	Vector2<int> _pos;
	Rect _rect;

	unsigned int _dispTime;
	unsigned int _appearTime;

public:
	virtual ~Enemy();

	void Draw();

	Vector2<int> GetPos()const;
	Rect GetRect()const;
};

