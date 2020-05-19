#include <DxLib.h>
#include "SpecialEnemy.h"

SpecialEnemy::SpecialEnemy(const int& dispTime, const int& appearTime,
						   const Vector2<int>& pos) : _point(60)
{
	_dispTime	= dispTime;
	_appearTime = appearTime;
	_pos		= pos;

	/// ‹éŒ`‚Ìİ’è(‰¼)
	_rect = Rect(_pos, Size(50, 50));
}

SpecialEnemy::~SpecialEnemy()
{
}

void SpecialEnemy::Draw()
{
	if (_dispTime <= 0)
	{
		DrawBox(_rect.center.x - _rect.size.width / 2, _rect.center.y - _rect.size.height / 2,
				_rect.center.x + _rect.size.width / 2, _rect.center.y + _rect.size.height / 2,
				0xff0000, true);
	}
}

int SpecialEnemy::GetPoint() const
{
	return _point;
}
