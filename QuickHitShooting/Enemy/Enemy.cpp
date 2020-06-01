#include <DxLib.h>
#include "Enemy.h"

Enemy::~Enemy()
{
}

/// 的の更新
void Enemy::Update()
{
	if (_dispTime <= 0)
	{
		--_banishTime;
	}
	else
	{
		--_dispTime;
	}
}

/// 的の描画
void Enemy::Draw()
{
	if (_dispTime <= 0)
	{
		if (_isAlive)
		{
			DrawExtendGraph(_rect.Left(), _rect.Top(), _rect.Right(), _rect.Bottom(), _targetImg, true);
		}
		else
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);
			DrawExtendGraph(_rect.Left(), _rect.Top(), _rect.Right(), _rect.Bottom(), _targetImg, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

/// 弾が当たったかの判定取得用
bool Enemy::HitShot()
{
	if (_dispTime <= 0 && _isAlive)
	{
		_isAlive	= false;
		_banishTime = 60;
		return true;
	}
	return false;
}

/// 的を消すかの判定取得用
bool Enemy::Destroy()
{
	return _banishTime <= 0;
}

/// 座標取得用
Vector2<int> Enemy::GetPos() const
{
	return _pos;
}

/// 矩形取得用
Rect Enemy::GetRect() const
{
	return _rect;
}