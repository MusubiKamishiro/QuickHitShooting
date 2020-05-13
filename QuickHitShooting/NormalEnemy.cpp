#include "NormalEnemy.h"

NormalEnemy::NormalEnemy(const int& dispTime, const int& appearTime, const Vector2<int>& pos)
{
	_dispTime	= dispTime;
	_appearTime = appearTime;
	_pos		= pos;

	/// 矩形の設定(仮)
	_rect = Rect(_pos, Size(50, 50));
}

NormalEnemy::~NormalEnemy()
{
}
