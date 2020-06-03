#include "DeductionEnemy.h"

DeductionEnemy::DeductionEnemy(const int& dispTime, const int& banishTime, 
							   const Vector2<int>& pos) : _point(-30)
{
	/// 的の初期化
	_dispTime	= dispTime;
	_banishTime = banishTime;
	_pos		= pos;
	_isAlive	= true;

	/// 画像IDの取得
	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/dEnemy.png", data);
	_targetImg = data.GetHandle();

	/// 矩形の設定
	_rect = Rect(_pos, Size(75, 75));
}

DeductionEnemy::~DeductionEnemy()
{
}

/// 得点取得用(減点の的)
int DeductionEnemy::GetScore() const
{
	return _point;
}
