#include <DxLib.h>
#include "NormalEnemy.h"

NormalEnemy::NormalEnemy(const int& dispTime, const int& appearTime, 
						 const Vector2<int>& pos) : _point(30)
{
	_dispTime	= dispTime;
	_appearTime = appearTime;
	_pos		= pos;
	_isAlive	= true;

	/// 矩形の設定(仮)
	_rect = Rect(_pos, Size(50, 50));
}

NormalEnemy::~NormalEnemy()
{
}

void NormalEnemy::Draw()
{
	if (_dispTime <= 0)
	{
		int color = (_isAlive ? 0xffff00 : 0x000000);
		DrawBox(_rect.center.x - _rect.size.width / 2, _rect.center.y - _rect.size.height / 2,
				_rect.center.x + _rect.size.width / 2, _rect.center.y + _rect.size.height / 2,
				color, true);
	}
}

int NormalEnemy::GetPoint() const
{
	return _point;
}
