#include <DxLib.h>
#include "DeductionEnemy.h"

DeductionEnemy::DeductionEnemy(const int& dispTime, const int& banishTime, 
							   const Vector2<int>& pos) : _point(-30)
{
	_dispTime	= dispTime;
	_banishTime = banishTime;
	_pos		= pos;
	_isAlive	= true;

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/dEnemy.png", data);
	_targetImg = data.GetHandle();

	/// ‹éŒ`‚Ìİ’è(‰¼)
	_rect = Rect(_pos, Size(50, 50));
}

DeductionEnemy::~DeductionEnemy()
{
}

void DeductionEnemy::Draw()
{
	if (_dispTime <= 0)
	{
		DrawExtendGraph(_rect.Left(), _rect.Top(), _rect.Right(), _rect.Bottom(), _targetImg, true);
	}
}

int DeductionEnemy::GetScore() const
{
	return _point;
}
