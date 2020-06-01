#include <DxLib.h>
#include "SpecialEnemy.h"

SpecialEnemy::SpecialEnemy(const int& dispTime, const int& banishTime,
						   const Vector2<int>& pos) : _point(60)
{
	_dispTime	= dispTime;
	_banishTime = banishTime;
	_pos		= pos;
	_isAlive	= true;

	/// ãÈå`ÇÃê›íË(âº)
	_rect = Rect(_pos, Size(50, 50));
}

SpecialEnemy::~SpecialEnemy()
{
}

void SpecialEnemy::Draw()
{
	if (_dispTime <= 0)
	{
		/// ÉqÉbÉgéûÇÕçïÇ≠ìhÇËÇ¬Ç‘Ç∑
		int color = (_isAlive ? 0xff0000 : 0x000000);
		DrawBox(_rect.center.x - _rect.size.width / 2, _rect.center.y - _rect.size.height / 2,
				_rect.center.x + _rect.size.width / 2, _rect.center.y + _rect.size.height / 2,
				color, true);
	}
}

int SpecialEnemy::GetScore() const
{

	return _point;
}
