#include "SpecialEnemy.h"

SpecialEnemy::SpecialEnemy(const int& dispTime, const int& appearTime, const Vector2<int>& pos)
{
	_dispTime	= dispTime;
	_appearTime = appearTime;
	_pos		= pos;

	/// ��`�̐ݒ�(��)
	_rect = Rect(_pos, Size(50, 50));
}

SpecialEnemy::~SpecialEnemy()
{
}
