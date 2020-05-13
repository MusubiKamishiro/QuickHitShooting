#include "DeductionEnemy.h"

DeductionEnemy::DeductionEnemy(const unsigned int& dispTime, const unsigned int& appearTime, const Vector2<int>& pos)
{
	_dispTime = dispTime;
	_appearTime = appearTime;
	_pos = pos;

	/// ‹éŒ`‚Ìİ’è(‰¼)
	_rect = Rect(_pos, Size(50, 50));
}

DeductionEnemy::~DeductionEnemy()
{
}
