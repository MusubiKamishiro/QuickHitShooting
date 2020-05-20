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

	Vector2<int> pos;
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

	void ChagneState(TargetState* targetState);
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

	bool IsReset();
	bool IsSave();
	bool IsLoad();

	void Draw();

	// �E�F�[�u���̃J�E���g�ۑ��p
	int _waveCnt;
	std::vector<int> _targetCnts;
	int _targetCnt;

	int _nowWaveCnt, _nowTargetCnt;

	// �E�F�[�u�̃f�[�^�ێ��p�ϐ�
	vec2_target _stageData;

	// ���͊֌W�̃|�C���^�[
	unique_input _input;

	unique_state _targetState;

	// �G�f�B�b�g���[�h�̊֐��|�C���^�[
	void (Stage::* _nowMode)();

	static std::unique_ptr<Stage, EditerDeleter> s_Instance;

	const Vector2<int> _gameScreen;
	const Vector2<int> _screen;
	const int _targetCntMax;	// �I�̍ő吔(����A5��)

	const char _waveEnd;
};