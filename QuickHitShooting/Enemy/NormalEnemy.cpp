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
	ImageData iData;
	Game::Instance().GetFileSystem()->Load("img/nEnemy.png", iData);
	_targetImg = iData.GetHandle();

	SoundData sData;
	Game::Instance().GetFileSystem()->Load("sound/se/normal.mp3", sData);
	Game::Instance().GetSoundPlayer()->AddSound("normal", sData.GetHandle(), 90);

	/// ��`�̐ݒ�
	_rect = Rect(_pos, Size(75, 75));

	InitImgElem();
}

NormalEnemy::~NormalEnemy()
{
}

void NormalEnemy::HitSound()
{
	Game::Instance().GetSoundPlayer()->PlaySound("normal");
}

/// ���_�擾�p(�ʏ�̓I)
int NormalEnemy::GetScore() const
{
	return _point;
}
