#include "NormalEnemy.h"

NormalEnemy::NormalEnemy(const int& dispTime, const int& banishTime, 
						 const Vector2<int>& pos) : _point(30)
{
	/// �I�̏�����
	_dispTime	= dispTime;
	_banishTime = banishTime;
	_pos		= pos;
	_isAlive	= true;

	/// �摜ID�̎擾
	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/nEnemy.png", data);
	_targetImg = data.GetHandle();

	/// ��`�̐ݒ�
	_rect = Rect(_pos, Size(75, 75));
}

NormalEnemy::~NormalEnemy()
{
}

/// ���_�擾�p(�ʏ�̓I)
int NormalEnemy::GetScore() const
{
	return _point;
}
