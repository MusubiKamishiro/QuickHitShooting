#include "Enemy.h"

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	if (_dispTime <= 0)
	{
		--_appearTime;
	}
	else
	{
		--_dispTime;
	}
}

void Enemy::HitShot()
{
	if (_dispTime <= 0)
	{
		_appearTime = 0;
	}
}

bool Enemy::Destroy()
{
	return _appearTime <= 0;
}

Vector2<int> Enemy::GetPos() const
{
	return _pos;
}

Rect Enemy::GetRect() const
{
	return _rect;
}
