#pragma once
#include "Enemy.h"
class DeductionEnemy :
	public Enemy
{
public:
	DeductionEnemy(const int& dispTime, const int& appearTime,
				   const Vector2<int>& pos);
	~DeductionEnemy();
};

