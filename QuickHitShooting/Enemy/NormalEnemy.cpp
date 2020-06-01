#include "NormalEnemy.h"

NormalEnemy::NormalEnemy(const int& dispTime, const int& banishTime, 
						 const Vector2<int>& pos) : _point(30)
{
	/// 的の初期化
	_dispTime	= dispTime;
	_banishTime = banishTime;
	_pos		= pos;
	_isAlive	= true;

	/// 画像IDの取得
	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/nEnemy.png", data);
	_targetImg = data.GetHandle();

	/// 矩形の設定
	_rect = Rect(_pos, Size(75, 75));
}

NormalEnemy::~NormalEnemy()
{
}

/// 得点取得用(通常の的)
int NormalEnemy::GetScore() const
{
	return _point;
}
