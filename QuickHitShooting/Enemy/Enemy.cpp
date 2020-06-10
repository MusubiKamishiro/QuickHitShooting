#include <DxLib.h>
#include "Enemy.h"
#include "../SoundPlayer.h"

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

void Enemy::InitImgElem()
{
	Vector2<int> midPoint = { _pos.x - (_eSize.width / 2), _pos.y - (_eSize.height / 2) };
	Vector2<int> offset;
	Vector2<float> vec;
	float cost, sint;
	for (int i = 0; i < _imgElements.size(); ++i)
	{
		offset.x = (i % 5) * (_eSize.width / 5);
		offset.y = (i / 5) * (_eSize.height / 5);

		_imgElements[i].pos.x = midPoint.x + offset.x;
		_imgElements[i].pos.y = midPoint.y + offset.y;

		vec = { _imgElements[i].pos.x - (midPoint.x + (_eSize.width / 2)),
				_imgElements[i].pos.y - (midPoint.y + (_eSize.height / 2)) };
		vec.Normalize();

		_imgElements[i].deg = 0;

		_imgElements[i].vel.x = vec.x * 4.f;
		_imgElements[i].vel.y = vec.y * 4.f;
	}
}

/// 的の描画
void Enemy::Draw()
{
	if (_dispTime <= 0)
	{
		if (_isAlive)
		{
			float extRate = (float)_eSize.width / 150;
			float sita;
			for (int i = 0; i < _imgElements.size(); ++i)
			{
				/// 現状、画像サイズは直値にしている
				sita = _imgElements[i].deg * DX_TWO_PI / 360;
				DrawRectRotaGraph(_imgElements[i].pos.x, _imgElements[i].pos.y,
					30 * (i % 5), 30 * (i / 5), 30, 30, extRate, sita, _targetImg, true);
			}
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
		HitSound();
		_isAlive	= false;
		_banishTime = 30;
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