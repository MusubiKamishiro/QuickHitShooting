#pragma once
#include "Enemy.h"
class DeductionEnemy :
	public Enemy
{
private:
	const int _point;
public:
	DeductionEnemy(const int& dispTime, const int& appearTime,
				   const Vector2<int>& pos);
	~DeductionEnemy();

	void Draw() override;

	int GetPoint() const override;
};

