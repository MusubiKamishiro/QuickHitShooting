#pragma once
#include "Enemy.h"
class NormalEnemy :
	public Enemy
{
private:
	const int _point;
public:
	NormalEnemy(const int& dispTime, const int& banishTime,
				const Vector2<int>& pos);
	~NormalEnemy();

	void Draw() override;

	int GetScore() const override;
};

