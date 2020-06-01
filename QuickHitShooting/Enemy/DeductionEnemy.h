#pragma once
#include "Enemy.h"
class DeductionEnemy :
	public Enemy
{
private:
	const int _point;
public:
	DeductionEnemy(const int& dispTime, const int& banishTime,
				   const Vector2<int>& pos);
	~DeductionEnemy();

	int GetScore() const override;
};

