#include <DxLib.h>
#include "DeductionEnemy.h"

DeductionEnemy::DeductionEnemy(const int& dispTime, const int& appearTime, 
							   const Vector2<int>& pos) : _point(-30)
{
	_dispTime	= dispTime;
	_appearTime = appearTime;
	_pos		= pos;

	/// ãÈå`ÇÃê›íË(âº)
	_rect = Rect(_pos, Size(50, 50));
}

DeductionEnemy::~DeductionEnemy()
{
}

void DeductionEnemy::Draw()
{
	if (_dispTime <= 0)
	{
		DrawBox(_rect.center.x - _rect.size.width / 2, _rect.center.y - _rect.size.height / 2,
				_rect.center.x + _rect.size.width / 2, _rect.center.y + _rect.size.height / 2,
				0x0000ff, true);
	}
}

int DeductionEnemy::GetPoint() const
{
	return _point;
}
