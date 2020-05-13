#pragma once
#include "Enemy.h"
class SpecialEnemy :
	public Enemy
{
public:
	SpecialEnemy(const unsigned int& dispTime,
				 const unsigned int& appearTime,
				 const Vector2<int>& pos);
	~SpecialEnemy();
};

