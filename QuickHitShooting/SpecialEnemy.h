#pragma once
#include "Enemy.h"
class SpecialEnemy :
	public Enemy
{
private:
	const int _point;
public:
	SpecialEnemy(const int& dispTime, const int& appearTime,
				 const Vector2<int>& pos);
	~SpecialEnemy();

	void Draw() override;

	int GetScore() const override;
};

