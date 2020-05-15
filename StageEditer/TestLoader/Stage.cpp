#include <stdio.h>
#include <DxLib.h>
#include "Input.h"
#include "Stage.h"
#include "Target/TargetState.h"
#include "Target/TargetType.h"

std::unique_ptr<Stage, Stage::EditerDeleter> Stage::s_Instance(new Stage());

Stage::Stage() : _targetCntMax(10), _screen(1920, 1080), 
_gameScreen(1280, 720), _waveEnd(55)
{
	Init();
	_input = std::make_unique<Input>();
}

Stage::~Stage()
{
	_stageData.clear();
}

bool Stage::Init()
{
	ChangeWindowMode(true);
	SetGraphMode(_screen.x, _screen.y, 32);
	if (DxLib::DxLib_Init() == -1)
	{
		return false;
	}

	SetMainWindowText("StageEditer");
	SetDrawScreen(DX_SCREEN_BACK);

	Wave();

	return false;
}

void Stage::Wave()
{
	_nowMode = &Stage::WaveUpdate;
	_waveCnt = 1;

	_stageData.clear();
}

void Stage::Target()
{
	_nowMode = &Stage::TargetUpdate;
	// �E�F�[�u����0�̎��A�E�F�[�u����1�ɂ���
	_waveCnt = (_waveCnt == 0 ? 1 : _waveCnt);
	_stageData.reserve(_waveCnt);
	_stageData.resize(_waveCnt);

	_targetCnt = 3;
}

void Stage::Edit()
{
	/// �ݒ肷��I�̏�����
	_nowMode	 = &Stage::EditUpdate;
	_targetState = std::make_unique<TargetType>();

	/// ���݂̃E�F�[�u���̏�����
	_nowWaveCnt = _nowTargetCnt = 0;

	/// �E�F�[�u���Ƃ̃^�[�Q�b�g���̐ݒ�
	for (auto& wave : _stageData)
	{
		wave.reserve(_targetCnt);
		wave.resize(_targetCnt);

		for (auto& target : wave)
		{
			/// �I���̏�����
			target.type			= 0;
			target.appearTime	= 60;
			target.dispTime		= 60;
			target.pos.x		= _screen.x;
			target.pos.y		= _screen.y;
		}
	}

	/// �����T�C�Y�̏�����
	SetFontSize(32);
}

void Stage::WaveUpdate()
{
	if (_input->IsTrigger(KEY_INPUT_SPACE))
	{
		Target();
		return;
	}

	if (_input->IsTrigger(KEY_INPUT_DOWN))
	{
		_waveCnt = (_waveCnt > 1 ? _waveCnt - 1 : 1);
	}
	else if (_input->IsTrigger(KEY_INPUT_UP))
	{
		++_waveCnt;
	}
	else {}

	int strWidth, strHeight;
	strWidth = strHeight = 0;

	SetFontSize(80);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "���݂̃E�F�[�u��", strlen("���݂̃E�F�[�u��"));
	DrawString((_screen.x / 2) - (strWidth / 2), (_screen.y / 2) - strHeight, "���݂̃E�F�[�u��", 0xffffff);

	SetFontSize(140);
	DrawFormatString((_screen.x / 2), (_screen.y / 2) + strHeight, 0x88ff88, "%d", _waveCnt);
}

void Stage::TargetUpdate()
{
	if (_input->IsTrigger(KEY_INPUT_SPACE))
	{
		Edit();
		return;
	}

	if (_input->IsTrigger(KEY_INPUT_DOWN))
	{
		/// �I���̌��Z
		_targetCnt = (_targetCnt > 3 ? _targetCnt - 1 : 3);
	}
	else if (_input->IsTrigger(KEY_INPUT_UP))
	{
		/// �I���̉��Z
		_targetCnt = (_targetCnt < _targetCntMax ? _targetCnt + 1 : _targetCntMax);
	}
	else {}

	int strWidth, strHeight;
	strWidth = strHeight = 0;

	SetFontSize(80);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "�o������I�̐�", strlen("�o������I�̐�"));
	DrawString((_screen.x / 2) - (strWidth / 2), (_screen.y / 2) - strHeight, "�o������I�̐�", 0xffffff);

	SetFontSize(140);
	DrawFormatString((_screen.x / 2), (_screen.y / 2) + strHeight, 0x88ff88, "%d", _targetCnt);
}

void Stage::EditUpdate()
{
	/// �X�e�[�W�f�[�^�̏�����
	if (IsReset())
	{
		Wave();
		_stageData.clear();
		return;
	}

	/// ���[�h����
	if (IsLoad())
	{
		Load();
	}

	/// �Z�[�u����
	if (IsSave)
	{
		Save();
	}
	
	/// �^�[�Q�b�g���̍X�V
	_targetState->Update(_nowWaveCnt, _nowTargetCnt, _input, _stageData);
}

bool Stage::IsReset()
{
	if (_input->IsTrigger(KEY_INPUT_F1))
	{
		if (MessageBox(
			GetMainWindowHandle(),
			"�X�e�[�W�����ŏ�����ݒ肵�܂����H",
			"�X�e�[�W�f�[�^�̏�����",
			MB_OKCANCEL) == IDOK)
		{
			return true;
		}
	}
	return false;
}

bool Stage::IsSave()
{
	if (_input->IsTrigger(KEY_INPUT_F5))
	{
		if (MessageBox(
			GetMainWindowHandle(),
			"�X�e�[�W�f�[�^���Z�[�u���܂����H",
			"�Z�[�u�̊m�F",
			MB_OKCANCEL) == IDOK)
		{
			return true;
		}
	}
	return false;
}

bool Stage::IsLoad()
{
	if (_input->IsTrigger(KEY_INPUT_F4))
	{
		if (MessageBox(
			GetMainWindowHandle(),
			"�X�e�[�W�f�[�^�����[�h���܂����H",
			"���[�h�̊m�F",
			MB_OKCANCEL) == IDOK)
		{
			return true;
		}
	}
	return false;
}

bool Stage::Save()
{
	// �t�@�C���t�H���_�[���J���ď������ނ��߂̏�����
	char fileSize[MAX_PATH] = "";											// �t�@�C�����̃T�C�Y�ƍŌ��\0������
	ZeroMemory(&openFileName, sizeof(openFileName));						// �\���̂̏�����
	openFileName.lStructSize = sizeof(OPENFILENAME);						// �\���̂̑傫��
	openFileName.lpstrFilter = TEXT("bin�t�@�C��(*.bin)\0*.bin\0\0");		// �`���̑I��
	openFileName.lpstrFile   = fileSize;									// �J���t�@�C�����̒���
	openFileName.lpstrInitialDir = ("../");									// �J���t�H���_�̎w��
	openFileName.nMaxFile	 = MAX_PATH;									// �J���t�@�C���̑傫��
	openFileName.lpstrDefExt = (".bin");									// �ۑ�����Ƃ��̃t�@�C���`��

	if (GetSaveFileName(&openFileName) == true)
	{
		FILE* file;
		/// �t�H���_�[�Ŏw�肵���t�@�C�����J��
		if (fopen_s(&file, openFileName.lpstrFile, "wb") == 0)
		{
			/// �G�f�B�^�[�̉�ʃT�C�Y����Q�[���̉�ʃT�C�Y�̔{�������߂Ă���
			Vector2<double> rate = Vector2<double>((double)_gameScreen.x / _screen.x,
												   (double)_gameScreen.y / _screen.y);

			for (auto wave : _stageData)
			{
				for (auto target : wave)
				{
					/// �^�[�Q�b�g������������
					fwrite(&target.type, sizeof(target.type), 1, file);
					fwrite(&target.dispTime, sizeof(target.dispTime), 1, file);
					fwrite(&target.appearTime, sizeof(target.appearTime), 1, file);
					target.pos.x *= rate.x;
					fwrite(&target.pos.x, sizeof(target.pos.x), 1, file);
					target.pos.y *= rate.y;
					fwrite(&target.pos.y, sizeof(target.pos.y), 1, file);
				}
				/// �E�F�[�u���̃G���h�|�C���g����������
				fwrite(&_waveEnd, sizeof(_waveEnd), 1, file);
			}
			/// eof�̏�������
			char eof = -1;
			fwrite(&eof, sizeof(eof), 1, file);
			fclose(file);
		}
		else
		{
			MessageBox(GetMainWindowHandle(),
				"�t�@�C����������܂���ł����B",
				"Not Found File",
				MB_OK);
		}
	}
	return true;
}

bool Stage::Load()
{
	// �t�@�C���t�H���_�[���J���ēǂݍ��ނ��߂̏�����
	char fileSize[MAX_PATH]	 = "";											// �t�@�C�����̃T�C�Y�ƍŌ��\0������
	ZeroMemory(&openFileName, sizeof(openFileName));						// �\���̂̏�����
	openFileName.lStructSize = sizeof(OPENFILENAME);						// �\���̂̑傫��
	openFileName.lpstrFilter = TEXT("bin�t�@�C��(*.bin)\0*.bin\0\0");		// �`���̑I��
	openFileName.lpstrFile	 = fileSize;									// �J���t�@�C�����̒���
	openFileName.lpstrInitialDir = ("../");									// �J���t�H���_�̎w��
	openFileName.nMaxFile	 = MAX_PATH;									// �J���t�@�C���̑傫��
	openFileName.lpstrDefExt = (".bin");									// �ۑ�����Ƃ��̃t�@�C���`��

	if (GetSaveFileName(&openFileName) == true)
	{
		// �Ƃ肠�����f�[�^�̏��������s��
		_stageData.clear();
		FILE* file;
		/// �t�H���_�[�Ŏw�肵���t�@�C�����J��
		if (fopen_s(&file, openFileName.lpstrFile, "rb") == 0)
		{
			/// �Q�[���̉�ʃT�C�Y����G�f�B�^�[�̉�ʃT�C�Y�̔{�������߂Ă���
			Vector2<double> rate = Vector2<double>((double)_gameScreen.x / _screen.x,
				(double)_gameScreen.y / _screen.y);

			TargetData target;
			std::vector<TargetData> targetData;
			int bytePos = 0;
			char checkVal = 0;

			fseek(file, bytePos, SEEK_SET);
			while (checkVal != -1)
			{
				/// �ǂݍ��ރo�C�g�Ԓn���w�肷��
				fseek(file, bytePos, SEEK_SET);

				{/* �I���̓ǂݍ��� */

					/// �I�̎��ID
					fread(&target.type, sizeof(target.type), 1, file);
					bytePos += sizeof(target.type);

					/// �I�̏o�����鎞��
					fread(&target.dispTime, sizeof(target.dispTime), 1, file);
					bytePos += sizeof(target.dispTime);

					/// �I���o�����Ă��������܂ł̎���
					fread(&target.appearTime, sizeof(target.appearTime), 1, file);
					bytePos += sizeof(target.appearTime);

					/// �I��X���W
					fread(&target.pos.x, sizeof(target.pos.x), 1, file);
					target.pos.x *= rate.x;
					bytePos += sizeof(target.pos.x);

					/// �I��Y���W
					fread(&target.pos.y, sizeof(target.pos.y), 1, file);
					target.pos.y *= rate.y;
					bytePos += sizeof(target.pos.y);
				}

				/// �I���̓o�^
				targetData.push_back(target);

				/// �E�F�[�u���̖������̊m�F���s��
				fread(&checkVal, sizeof(checkVal), 1, file);
				if (checkVal == _waveEnd)
				{
					/// 1�E�F�[�u�ŏo������I���̓o�^
					bytePos += sizeof(checkVal);
					_stageData.push_back(targetData);
					targetData.clear();
				}

				/// eof�̊m�F���s�����߂̓ǂݍ���
				fread(&checkVal, sizeof(checkVal), 1, file);
			}
			fclose(file);
		}
		else
		{
			MessageBox(GetMainWindowHandle(),
				"�t�@�C����������܂���ł����B",
				"Not Found File",
				MB_OK);
		}
	}
	return true;
}

void Stage::Update()
{
	while (ProcessMessage() == 0 && !_input->IsTrigger(KEY_INPUT_ESCAPE))
	{
		ClsDrawScreen();
		_input->Update();
		(this->*_nowMode)();
		Draw();

		ScreenFlip();
	}
}

void Stage::ChagneState(TargetState* targetState)
{
	_targetState.reset(targetState);
}

void Stage::Draw()
{
	/// �X�e�[�W�G�f�B�^�ŕK�v�ȏ���`�悷��\��
}
