#include <DxLib.h>
#include "Enemy.h"
#include "../SoundPlayer.h"

Enemy::~Enemy()
{
}

/// �I�̍X�V
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

/// �I�̕`��
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
				/// ����A�摜�T�C�Y�͒��l�ɂ��Ă���
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

/// �e�������������̔���擾�p
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

/// �I���������̔���擾�p
bool Enemy::Destroy()
{
	return _banishTime <= 0;
}

/// ���W�擾�p
Vector2<int> Enemy::GetPos() const
{
	return _pos;
}

/// ��`�擾�p
Rect Enemy::GetRect() const
{
	return _rect;
}