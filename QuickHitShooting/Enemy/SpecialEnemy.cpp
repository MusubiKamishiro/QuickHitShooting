#include "SpecialEnemy.h"

SpecialEnemy::SpecialEnemy(const int& dispTime, const int& banishTime,
						   const Vector2<int>& pos) : _point(60)
{
	/// 的の初期化
	_dispTime	= dispTime;
	_banishTime = banishTime;
	_pos		= pos;
	_isAlive	= true;

	/// 画像IDの取得
	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/sEnemy.png", data);
	_targetImg = data.GetHandle();

	/// 矩形の設定
	_rect = Rect(_pos, Size(75, 75));
}

SpecialEnemy::~SpecialEnemy()
{
}

/// 得点取得用(特別な的)
int SpecialEnemy::GetScore() const
{
	return _point;
}
