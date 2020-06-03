#pragma once
#include "Enemy.h"
class SpecialEnemy :
	public Enemy
{
private:
	const int _point;
public:
	SpecialEnemy(const int& dispTime, const int& banishTime,
				 const Vector2<int>& pos);
	~SpecialEnemy();

	int GetScore() const override;
};

