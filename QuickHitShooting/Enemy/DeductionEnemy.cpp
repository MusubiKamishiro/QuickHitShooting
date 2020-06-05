#include "DeductionEnemy.h"

DeductionEnemy::DeductionEnemy(const int& dispTime, const int& banishTime, 
							   const Vector2<int>& pos) : _point(-30)
{
	/// �I�̏�����
	_dispTime	= dispTime;
	_banishTime = banishTime;
	_pos		= pos;
	_isAlive	= true;

	/// �摜ID�̎擾
	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/dEnemy.png", data);
	_targetImg = data.GetHandle();

	SoundData sData;
	Game::Instance().GetFileSystem()->Load("sound/se/deduction.mp3", sData);
	Game::Instance().GetSoundPlayer()->AddSound("deduction", sData.GetHandle());

	/// ��`�̐ݒ�
	_rect = Rect(_pos, Size(75, 75));
}

DeductionEnemy::~DeductionEnemy()
{
}

void DeductionEnemy::HitSound()
{
	Game::Instance().GetSoundPlayer()->PlaySound("deduction");
}

/// ���_�擾�p(���_�̓I)
int DeductionEnemy::GetScore() const
{
	return _point;
}
