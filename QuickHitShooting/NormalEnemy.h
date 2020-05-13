#pragma once
#include "Enemy.h"
class NormalEnemy :
	public Enemy
{
public:
	NormalEnemy(const int& dispTime, const int& appearTime,
				const Vector2<int>& pos);
	~NormalEnemy();
};

