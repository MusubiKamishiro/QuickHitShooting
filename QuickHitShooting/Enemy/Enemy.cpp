#include <DxLib.h>
#include "Enemy.h"
#include "../SoundPlayer.h"

Enemy::~Enemy()
{
}

/// 分割した画像情報を初期化する
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

/// 的が壊れた時のアニメーションを更新する
void Enemy::BreakUpdate()
{
	for (auto& elem : _imgElements)
	{
		elem.pos   += elem.vel;
		elem.vel.y += 0.6f;

		elem.deg = (elem.vel.x >= 0 ? elem.deg + 15 : elem.deg - 15);

		/// 1回転した時の角度調整
		if (elem.deg >= 360)
		{
			elem.deg = 0;
		}
		else if (elem.deg <= 0)
		{
			elem.deg = 360;
		}
		else {}
	}
}

/// 的の更新
void Enemy::Update()
{
	if (!_isAlive)
	{
		BreakUpdate();
	}
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
		float extRate = (float)_eSize.width / 150;
		if (_isAlive)
		{
			for (int i = 0; i < _imgElements.size(); ++i)
			{
				/// 分割した時の画像サイズは現状直値
				DrawRectRotaGraph(_imgElements[i].pos.x, _imgElements[i].pos.y,
								  30 * (i % 5), 30 * (i / 5), 30, 30, extRate, 0, 
								  _targetImg, true);
			}
		}
		else
		{
			/// 透明度の計算
			float rate = (float)_banishTime / _deathTime;
			int alpha  = 255 * rate;
			float sita;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			for (int i = 0; i < _imgElements.size(); ++i)
			{
				/// 分割した時の画像サイズは現状直値
				sita = _imgElements[i].deg * DX_TWO_PI / 360;
				DrawRectRotaGraph(_imgElements[i].pos.x, _imgElements[i].pos.y,
								  30 * (i % 5), 30 * (i / 5), 30, 30, extRate, sita,
								  _targetImg, true);
			}
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
		_banishTime = _deathTime;
		return true;
	}
	return false;
}

/// 敵の削除を行うかの判定用
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