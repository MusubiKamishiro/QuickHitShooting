#include "SpecialEnemy.h"

SpecialEnemy::SpecialEnemy(const int& dispTime, const int& banishTime,
						   const Vector2<int>& pos) : _point(60)
{
	/// �I�̏�����
	_dispTime	= dispTime;
	_banishTime = banishTime;
	_pos		= pos;
	_isAlive	= true;

	/// �摜ID�̎擾
	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/sEnemy.png", data);
	_targetImg = data.GetHandle();

	/// ��`�̐ݒ�
	_rect = Rect(_pos, Size(75, 75));
}

SpecialEnemy::~SpecialEnemy()
{
}

/// ���_�擾�p(���ʂȓI)
int SpecialEnemy::GetScore() const
{
	return _point;
}
