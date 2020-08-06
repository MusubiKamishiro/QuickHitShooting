#pragma once
#include <memory>
#include "../Geometry.h"

class Peripheral;

// (�������z�N���X)
class Scene
{
protected:

	void DrawEffect();

	Vector2<int> _efkPos;

	// �`��u�����h���[�h�̃p�����[�^(0~255)
	int _pal;
	// ��ʂ̃T�C�Y
	Vector2<int> _scrSize;
	// �V�[���J�n����̌o�ߎ���
	int _sceneTime;

	/// �e�𔭎˂������̃G�t�F�N�g
	int _shotEffect;

	/// �Đ����̃G�t�F�N�g
	int _playEffect;

public:
	Scene();
	virtual ~Scene();

	// �V�[���̍X�V���s��
	virtual void Update(const Peripheral& p) = 0;
	// �V�[���̕`��
	virtual void Draw() = 0;
	
	// �t�F�[�h�C���̏���������
	virtual void FadeinUpdate(const Peripheral& p) = 0;
	// �t�F�[�h�A�E�g�̏���������
	virtual void FadeoutUpdate(const Peripheral& p) = 0;
	// �ҋ@���̏���������
	virtual void WaitUpdate(const Peripheral& p) = 0;

	// �V�[���̐i�s���Ԃ��v���Ă���
	void AddSceneTime();
};

