#include "Enemy.h"
#include <DxLib.h>

Enemy::~Enemy()
{
}

void Enemy::Draw()
{
	DxLib::DrawBox(_rect.center.x - _rect.size.width/2, _rect.center.y - _rect.size.height / 2,
		_rect.center.x + _rect.size.width / 2, _rect.center.y + _rect.size.height / 2, 0x00ff00, true);
}

Vector2<int> Enemy::GetPos() const
{
	return _pos;
}

Rect Enemy::GetRect() const
{
	return _rect;
}
