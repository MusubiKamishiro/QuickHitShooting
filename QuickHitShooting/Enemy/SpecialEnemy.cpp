#include <DxLib.h>
#include "SpecialEnemy.h"

SpecialEnemy::SpecialEnemy(const int& dispTime, const int& banishTime,
						   const Vector2<int>& pos) : _point(60)
{
	_dispTime	= dispTime;
	_banishTime = banishTime;
	_pos		= pos;
	_isAlive	= true;

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/sEnemy.png", data);
	_targetImg = data.GetHandle();

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
		DrawExtendGraph(_rect.Left(), _rect.Top(), _rect.Right(), _rect.Bottom(), _targetImg, true);
	}
}

int SpecialEnemy::GetScore() const
{

	return _point;
}
