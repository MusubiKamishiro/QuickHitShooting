#include "Enemy.h"
#include <DxLib.h>

Enemy::Enemy()
{
	pos = Vector2<int>(50, 50);
	rect = Rect(pos, Size(50, 50));
}

Enemy::~Enemy()
{
}

void Enemy::Draw()
{
	DxLib::DrawBox(rect.center.x - rect.size.width/2, rect.center.y - rect.size.height / 2,
		rect.center.x + rect.size.width / 2, rect.center.y + rect.size.height / 2, 0x00ff00, true);
}

Vector2<int> Enemy::GetPos() const
{
	return pos;
}

Rect Enemy::GetRect() const
{
	return rect;
}
