#pragma once
#include "Enemy.h"
class DeductionEnemy :
	public Enemy
{
	DeductionEnemy(const unsigned int& dispTime,
				   const unsigned int& appearTime,
				   const Vector2<int>& pos);
	~DeductionEnemy();
};

