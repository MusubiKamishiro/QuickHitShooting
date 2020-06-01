#include <DxLib.h>
#include "DeductionEnemy.h"

DeductionEnemy::DeductionEnemy(const int& dispTime, const int& banishTime, 
							   const Vector2<int>& pos) : _point(-30)
{
	_dispTime	= dispTime;
	_banishTime = banishTime;
	_pos		= pos;
	_isAlive	= true;

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
		/// ÉqÉbÉgéûÇÕçïÇ≠ìhÇËÇ¬Ç‘Ç∑
		int color = (_isAlive ? 0x0000ff : 0x000000);
		DrawBox(_rect.center.x - _rect.size.width / 2, _rect.center.y - _rect.size.height / 2,
				_rect.center.x + _rect.size.width / 2, _rect.center.y + _rect.size.height / 2,
				 color, true);
	}
}

int DeductionEnemy::GetScore() const
{
	return _point;
}
