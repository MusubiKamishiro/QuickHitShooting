#pragma once

#include <memory>
#include <vector>
#include <array>
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
	unsigned char type;			// �I�̎��		(1�Ԗڂ̃f�[�^)
	unsigned int appearTime;	// �o�����鎞�� (2�Ԗڂ̃f�[�^)
	unsigned int dispTime;		// �\�����鎞�� (3�Ԗڂ̃f�[�^)
	Vector2<int> pos;			// ���W			(4�Ԗڂ̃f�[�^)
};

using vec_target = std::vector<TargetData>;

using unique_input = std::unique_ptr<Input>;

// �I�ݒ�̏�ԑJ�ڗp�|�C���^�[
using unique_state = std::unique_ptr<TargetState>;

struct StageInfo
{
	// �E�F�[�u�̃f�[�^�ێ��p�ϐ�
	std::vector<vec_target> targetData;
	std::array<int, 3> scores;
	std::array<std::string, 3> names;
};

class Editer
{
public:
	static Editer& Instance()
	{
		/// �V���O���g���̒��g����̎��ɐ�������
		if (!s_Instance)
		{
			s_Instance.reset(new Editer());
		}
		return *s_Instance;
	}
	void Update();

	// �ݒ肷��I���̕ύX�p
	void ChagneState(TargetState* targetState);
	
	// �G�f�B�^�[�̉�ʃT�C�Y�擾�p
	Vector2<int> GetScreenSize() const;
private:
	Editer();
	~Editer();

	/// �V���O���g���̃f���[�^�[
	struct EditerDeleter
	{
		void operator()(Editer* pointer)
		{
			delete pointer;
		}
	};

	static std::unique_ptr<Editer, EditerDeleter> s_Instance;

	bool Init();

	/// �X�e�[�W�ݒ�̏������p
	void Wave();
	void Target();
	void Stage();

	/// �X�e�[�W�ݒ�̍X�V�p
	void WaveUpdate();
	void TargetUpdate();
	void StageUpdate();

	/// �X�e�[�W�f�[�^�̕`��p
	void WaveDrawer();
	void TargetDrawer();
	void StageDrawer();

	/// �X�e�[�W�f�[�^�̕ۑ��Ɠǂݍ��ݗp
	bool Save();
	bool Load();

	/// ����̍s�����s�����̔���p
	bool IsReset();
	bool IsSave();
	bool IsLoad();

	int _waveCnt;						// �E�F�[�u���̃J�E���g�ۑ��p
	int _configTarget;					// �ݒ蒆�̃^�[�Q�b�g
	std::vector<int> _waveTargetCnt;	// 1�E�F�[�u���̃^�[�Q�b�g��

	// �X�e�[�W�ݒ�Ŏg�p������
	int _nowWaveCnt, _nowTargetCnt;

	// ���͊֌W�̃|�C���^�[
	unique_input _input;

	unique_state _targetState;

	StageInfo _stageInfo;

	// �G�f�B�b�g���[�h�̊֐��|�C���^�[
	void (Editer::* _nowMode)();
	void (Editer::* _drawer)();

	const Vector2<int> _gameScreen;	// �Q�[���̉�ʃT�C�Y
	const Vector2<int> _screen;		// �G�f�B�^�[�̉�ʃT�C�Y
	const int _targetCntMax;		// �I�̍ő吔

	const char _waveEnd;			// �o�C�i����ŃE�F�[�u�̏I�����m�F���邽�߂̂���
};