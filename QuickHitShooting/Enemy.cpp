#include "Enemy.h"

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	if (_dispTime <= 0)
	{
		--_banishTime;
	}
	else
	{
		--_dispTime;
	}
}

bool Enemy::HitShot()
{
	if (_dispTime <= 0 && _isAlive)
	{
		_isAlive	= false;
		_banishTime = 60;
		return true;
	}
	return false;
}

bool Enemy::Destroy()
{
	return _banishTime <= 0;
}

Vector2<int> Enemy::GetPos() const
{
	return _pos;
}

Rect Enemy::GetRect() const
{
	return _rect;
}