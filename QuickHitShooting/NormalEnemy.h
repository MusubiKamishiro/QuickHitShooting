#pragma once
#include "Enemy.h"
class NormalEnemy :
	public Enemy
{
public:
	NormalEnemy(const unsigned int& dispTime,
				const unsigned int& appearTime,
				const Vector2<int>& pos);
	~NormalEnemy();
};

