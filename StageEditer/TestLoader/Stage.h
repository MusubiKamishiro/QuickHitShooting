#pragma once

#include <memory>
#include <vector>
#include <windows.h>
#include "Geometry.h"

class Input;
class TargetState;

enum class TargetID
{
	NORMAL,			// �ʏ�̓I
	SPECIAL,		// ���ʂȓI
	DEDUCTION,		// ���_�����I
	MAX
};

// �I�̏��
struct TargetData
{
	u_char type;			// �I�̎��		(1�Ԗڂ̃f�[�^)
	u_int appearTime;		// �o�����鎞�� (2�Ԗڂ̃f�[�^)
	u_int dispTime;			// �\�����鎞�� (3�Ԗڂ̃f�[�^)
	Vector2<int> pos;		// ���W			(4�Ԗڂ̃f�[�^)
};

using vec2_target  = std::vector<std::vector<TargetData>>;

using unique_input = std::unique_ptr<Input>;

// �I�̏�ԑJ�ڗp�|�C���^�[
using unique_state = std::unique_ptr<TargetState>;

class Stage
{
public:
	static Stage& Instance()
	{
		/// �V���O���g���̒��g����̎��ɐ�������
		if (!s_Instance)
		{
			s_Instance.reset(new Stage());
		}
		return *s_Instance;
	}
	void Update();

	// �ݒ肷��I���̕ύX�p
	void ChagneState(TargetState* targetState);
	
	// �G�f�B�^�[�̉�ʃT�C�Y�擾�p
	Vector2<int> GetScreenSize() const;
private:
	Stage();
	~Stage();

	struct EditerDeleter
	{
		void operator()(Stage* pointer)
		{
			delete pointer;
		}
	};

	static std::unique_ptr<Stage, EditerDeleter> s_Instance;

	bool Init();

	// �E�F�[�u���̐ݒ�p
	void Wave();

	void Target();

	// �X�e�[�W�̕ҏW
	void Edit();

	void WaveUpdate();
	void TargetUpdate();
	void EditUpdate();

	bool Save();
	bool Load();

	// �X�e�[�W�f�[�^�����������邩�̊m�F�p
	bool IsReset();

	// �ǂݍ��݂��s�����̊m�F�p
	bool IsSave();

	// �������݂��s�����̊m�F�p
	bool IsLoad();

	int _waveCnt;						// �E�F�[�u���̃J�E���g�ۑ��p
	int _configTarget;					// �ݒ蒆�̃^�[�Q�b�g
	std::vector<int> _waveTargetCnt;	// 1�E�F�[�u���̃^�[�Q�b�g��

	// �X�e�[�W�ݒ�Ŏg�p������
	int _nowWaveCnt, _nowTargetCnt;

	// �E�F�[�u�̃f�[�^�ێ��p�ϐ�
	vec2_target _stageData;

	// ���͊֌W�̃|�C���^�[
	unique_input _input;

	unique_state _targetState;

	// �G�f�B�b�g���[�h�̊֐��|�C���^�[
	void (Stage::* _nowMode)();

	const Vector2<int> _gameScreen;	// �Q�[���̉�ʃT�C�Y
	const Vector2<int> _screen;		// �G�f�B�^�[�̉�ʃT�C�Y
	const int _targetCntMax;		// �I�̍ő吔

	const char _waveEnd;			// �o�C�i����ŃE�F�[�u�̏I�����m�F���邽�߂̂���
};