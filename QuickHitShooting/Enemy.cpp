#include <DxLib.h>
#include "Enemy.h"

Enemy::~Enemy()
{
}

void Enemy::Draw()
{
	if (_dispTime <= 0)
	{
		DxLib::DrawBox(_rect.center.x - _rect.size.width / 2, _rect.center.y - _rect.size.height / 2,
					   _rect.center.x + _rect.size.width / 2, _rect.center.y + _rect.size.height / 2, 
						0x00ff00, true);
	}
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

bool Enemy::Destroy()
{
	return (_appearTime <= 0);
}

Vector2<int> Enemy::GetPos() const
{
	return _pos;
}

Rect Enemy::GetRect() const
{
	return _rect;
}
