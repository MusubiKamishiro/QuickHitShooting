#include <DxLib.h>
#include "Enemy.h"

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

/// �I�̕`��
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

/// �e�������������̔���擾�p
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