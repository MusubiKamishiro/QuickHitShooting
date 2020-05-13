#pragma once
#include "Enemy.h"
class SpecialEnemy :
	public Enemy
{
public:
	SpecialEnemy(const int& dispTime, const int& appearTime,
				 const Vector2<int>& pos);
	~SpecialEnemy();
};

