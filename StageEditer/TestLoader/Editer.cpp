#include <DxLib.h>
#include "Input.h"
#include "Editer.h"
#include "Target/TargetState.h"
#include "Target/TargetType.h"

std::unique_ptr<Editer, Editer::EditerDeleter> Editer::s_Instance(new Editer());

Editer::Editer() : _targetCntMax(10), _screen(1920, 1080), 
_gameScreen(1280, 720), _waveEnd(55)
{
	Init();
	_input = std::make_unique<Input>();
}

Editer::~Editer()
{
}

/// ��ʃE�B���h�E�Ȃǂ̏�����
bool Editer::Init()
{
	///	�E�B���h�E�̏�����
	ChangeWindowMode(true);
	SetGraphMode(_screen.x, _screen.y, 32);
	if (DxLib::DxLib_Init() == -1)
	{
		return false;
	}
	SetMainWindowText("StageEditer");
	SetDrawScreen(DX_SCREEN_BACK);

	/// �X�e�[�W�G�f�B�^�[�̏������
	Wave();

	return false;
}

/// �ݒ肷��I���̕ύX�p
void Editer::ChagneState(TargetState* targetState)
{
	_targetState.reset(targetState);
}

/// ��ʃT�C�Y�擾�p
Vector2<int> Editer::GetScreenSize() const
{
	return _screen;
}

/// �E�F�[�u�ݒ�̏�����
void Editer::Wave()
{
	_nowMode = &Editer::WaveUpdate;
	_drawer  = &Editer::WaveDrawer;
	_waveCnt = 1;

	/// �X�e�[�W�f�[�^�̏�����
	_stageInfo.targetData.clear();
	std::vector<vec_target>().swap(_stageInfo.targetData);
}

/// �^�[�Q�b�g�ݒ�̏�����
void Editer::Target()
{
	_nowMode = &Editer::TargetUpdate;
	_drawer  = &Editer::TargetDrawer;

	/// �E�F�[�u���̏�����
	_waveTargetCnt.clear();
	std::vector<int>().swap(_waveTargetCnt);

	/// �E�F�[�u���̐ݒ���s��
	_waveTargetCnt.reserve(_waveCnt);
	_waveTargetCnt.resize(_waveCnt);

	/// 1�E�F�[�u���Ƃ̓I���̏�����
	for (auto& tCnt : _waveTargetCnt)
	{
		tCnt = 3;
	}
}

/// �X�e�[�W�ݒ�̏�����
void Editer::Stage()
{
	/// �ݒ肷��I�̏�����
	_nowMode	 = &Editer::StageUpdate;
	_drawer		 = &Editer::StageDrawer;
	_targetState = std::make_unique<TargetType>();

	/// �E�F�[�u�̐���
	_stageInfo.targetData.reserve(_waveCnt);
	_stageInfo.targetData.resize(_waveCnt);

	/// �X�R�A�f�[�^�̏�����
	for (int i = 0; i < _stageInfo.scores.size(); ++i)
	{
		_stageInfo.scores[i] = 0;
		_stageInfo.names[i]  = "AAA";
	}

	/// �X�e�[�W�f�[�^�̏�����
	for (unsigned int i = 0; i < _stageInfo.targetData.size(); ++i)
	{
		/// 1�E�F�[�u���Ƃ̓I���̐ݒ�
		_stageInfo.targetData[i].reserve(_waveTargetCnt[i]);
		_stageInfo.targetData[i].resize(_waveTargetCnt[i]);

		/// �I���̏�����
		for (auto& target : _stageInfo.targetData[i])
		{
			target.type		  = 0;
			target.banishTime = 60;
			target.dispTime	  = 60;
			target.pos.x	  = _screen.x / 2;
			target.pos.y	  = _screen.y / 2;
		}
	}

	/// �G�f�B�^�[�Ŏg�p����l�̏�����
	_nowWaveCnt = _nowTargetCnt = 0;
}

/// �E�F�[�u�ݒ�̍X�V�p
void Editer::WaveUpdate()
{
	/// �I�ݒ�Ɉڍs����
	if (_input->IsTrigger(KEY_INPUT_SPACE))
	{
		Target();
		return;
	}

	/// �E�F�[�u���̐ݒ�
	if (_input->IsTrigger(KEY_INPUT_LEFT) ||
		_input->IsTrigger(KEY_INPUT_A))
	{
		_waveCnt = (_waveCnt > 1 ? _waveCnt - 1 : 1);
	}
	else if (_input->IsTrigger(KEY_INPUT_RIGHT) ||
			 _input->IsTrigger(KEY_INPUT_D))
	{
		++_waveCnt;
	}
	else {}
}

/// �I�ݒ�̍X�V�p
void Editer::TargetUpdate()
{
	/// �E�F�[�u�ݒ�Ɉڍs����
	if (_input->IsTrigger(KEY_INPUT_F1))
	{
		Wave();
		return;
	}

	/// �X�e�[�W�f�[�^�ݒ�Ɉڍs����
	if (_input->IsTrigger(KEY_INPUT_SPACE))
	{
		Stage();
		return;
	}

	/// �I���̐ݒ�
	if (_input->IsTrigger(KEY_INPUT_LEFT) ||
		_input->IsTrigger(KEY_INPUT_A))
	{
		_waveTargetCnt[_configTarget] = (_waveTargetCnt[_configTarget] > 3 ? _waveTargetCnt[_configTarget] - 1 : 3);
	}
	else if (_input->IsTrigger(KEY_INPUT_RIGHT) ||
			 _input->IsTrigger(KEY_INPUT_D))
	{
		_waveTargetCnt[_configTarget] = (_waveTargetCnt[_configTarget] < _targetCntMax ? _waveTargetCnt[_configTarget] + 1 : _targetCntMax);
	}
	else {}

	/// �ݒ肷��I�̐ؑ�
	if (_input->IsTrigger(KEY_INPUT_UP) ||
		_input->IsTrigger(KEY_INPUT_W))
	{
		int targetMax = _waveTargetCnt.size();
		_configTarget = ((_configTarget - 1) + targetMax) % targetMax;
	}
	else if (_input->IsTrigger(KEY_INPUT_DOWN) ||
			 _input->IsTrigger(KEY_INPUT_S))
	{
		int targetMax = _waveTargetCnt.size();
		_configTarget = (_configTarget + 1) % targetMax;
	}
	else{}
}

/// �X�e�[�W�ݒ�̍X�V�p
void Editer::StageUpdate()
{
	_targetState->Update(_nowWaveCnt, _nowTargetCnt, _input, _stageInfo.targetData);
	
	if (IsReset())
	{
		Wave();
		return;
	}

	if (IsLoad())
	{
		Load();
	}

	if (IsSave())
	{
		Save();
	}
}

/// �E�F�[�u�ݒ�̕`��
void Editer::WaveDrawer()
{
	Vector2<int> strSize;
	std::string text;
	SetFontSize(80);
	text = "Now Wave Count";
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) - strSize.y, text.c_str(), 0xffffff);

	/// ���݂̃E�F�[�u��
	SetFontSize(140);
	text = std::to_string(_waveCnt);
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) + (strSize.y / 2), text.c_str(), 0x88ff88);
}

/// �I�ݒ�̕`��
void Editer::TargetDrawer()
{
	Vector2<int> strSize;
	std::string text;
	SetFontSize(80);
	text = "Appear Target Count";
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) - strSize.y, text.c_str(), 0xffffff);

	/// �ݒ���s���Ă���I�̕\��
	int nowTargetColor;
	for (int i = 0; i < _waveTargetCnt.size(); ++i)
	{
		nowTargetColor = (_configTarget == i ? 0xffff00 : 0xffffff);
		text = std::to_string(i + 1) + " : " + std::to_string(_waveTargetCnt[i]);
		GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
		DrawString(_screen.x - strSize.x, strSize.y * i, text.c_str(), nowTargetColor);
	}

	/// ���݂̓I��
	SetFontSize(140);
	text = std::to_string(_waveTargetCnt[_configTarget]);
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) + (strSize.y / 2), text.c_str(), 0x88ff88);
}

/// �X�e�[�W�ݒ�̕`��
void Editer::StageDrawer()
{
	_targetState->Draw(_nowWaveCnt, _nowTargetCnt, _stageInfo.targetData);
}

/// �X�e�[�W�̍Đݒ���s�����̔���p
bool Editer::IsReset()
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

/// �X�e�[�W�f�[�^�̕ۑ����s�����̔���p
bool Editer::IsSave()
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

/// �X�e�[�W�f�[�^��ۑ��s�����̔���p
bool Editer::IsLoad()
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

/// �Z�[�u����
bool Editer::Save()
{
	/// �t�@�C���t�H���_�[���J���ď������ނ��߂̏�����
	OPENFILENAME openFileName;
	char fileSize[MAX_PATH] = "";											/// �t�@�C�����̃T�C�Y�ƍŌ��\0������
	ZeroMemory(&openFileName, sizeof(openFileName));						/// �\���̂̏�����
	openFileName.lStructSize = sizeof(OPENFILENAME);						/// �\���̂̑傫��
	openFileName.lpstrFilter = TEXT("bin�t�@�C��(*.bin)\0*.bin\0\0");		/// �`���̑I��(�o�C�i���`��)
	openFileName.lpstrFile	 = fileSize;									/// �J���t�@�C�����̒���
	openFileName.lpstrInitialDir = ("../");									/// �J���t�H���_�̎w��
	openFileName.nMaxFile	 = MAX_PATH;									/// �J���t�@�C���T�C�Y
	openFileName.lpstrDefExt = (".bin");									/// �ۑ�����Ƃ��̃t�@�C���`��

	if (GetSaveFileName(&openFileName) == true)
	{
		FILE* file;
		/// �t�H���_�[�Ŏw�肵���t�@�C�����J��
		if (fopen_s(&file, openFileName.lpstrFile, "wb") == 0)
		{
			char name[3];
			for (int i = 0; i < _stageInfo.scores.size(); ++i)
			{
				fwrite(&_stageInfo.scores[i], sizeof(int), 1, file);
				for (int c = 0; c < sizeof(name) / sizeof(name[0]); ++c)
				{
					/// �����̎擾���s���Ă���
					name[c] = _stageInfo.names[i][c];
				}
				fwrite(name, (sizeof(char) * 3), 1, file);
			}
			/// �G�f�B�^�[�̉�ʃT�C�Y����Q�[���̉�ʃT�C�Y�̔{�������߂Ă���
			Vector2<double> rate = Vector2<double>((double)_gameScreen.x / _screen.x,
												   (double)_gameScreen.y / _screen.y);

			Vector2<int> registPos;
			int targetCnt = 0;

			/// �������ރE�F�[�u���̐ݒ�
			int waveCnt = _stageInfo.targetData.size();
			fwrite(&waveCnt, sizeof(int), 1, file);

			for (int w = 0; w < waveCnt; ++w)
			{
				/// �������ރ^�[�Q�b�g���̐ݒ�
				targetCnt = _stageInfo.targetData[w].size();
				fwrite(&targetCnt, sizeof(int), 1, file);
				for (int t = 0; t < targetCnt; ++t)
				{
					/// �^�[�Q�b�g�f�[�^�̏�������
					fwrite(&_stageInfo.targetData[w][t].type,		 sizeof(unsigned char), 1, file);
					fwrite(&_stageInfo.targetData[w][t].dispTime,	 sizeof(unsigned int), 1, file);
					fwrite(&_stageInfo.targetData[w][t].banishTime,  sizeof(unsigned int), 1, file);
					registPos.x = (_stageInfo.targetData[w][t].pos.x * rate.x);
					fwrite(&registPos.x, sizeof(int), 1, file);
					registPos.y = (_stageInfo.targetData[w][t].pos.y * rate.x);
					fwrite(&registPos.y, sizeof(int), 1, file);
				}
			}
			/// �t�@�C�������
			fclose(file);
		}
	}
	return true;
}

/// ���[�h����
bool Editer::Load()
{
	/// �t�@�C���t�H���_�[���J���ď������ނ��߂̏�����
	OPENFILENAME openFileName;
	char fileSize[MAX_PATH] = "";											/// �t�@�C�����̃T�C�Y�ƍŌ��\0������
	ZeroMemory(&openFileName, sizeof(openFileName));						/// �\���̂̏�����
	openFileName.lStructSize = sizeof(OPENFILENAME);						/// �\���̂̑傫��
	openFileName.lpstrFilter = TEXT("bin�t�@�C��(*.bin)\0*.bin\0\0");		/// �`���̑I��(�o�C�i���`��)
	openFileName.lpstrFile = fileSize;									/// �J���t�@�C�����̒���
	openFileName.lpstrInitialDir = ("../");									/// �J���t�H���_�̎w��
	openFileName.nMaxFile = MAX_PATH;									/// �J���t�@�C���T�C�Y
	openFileName.lpstrDefExt = (".bin");									/// �ۑ�����Ƃ��̃t�@�C���`��

	if (GetSaveFileName(&openFileName) == true)
	{
		FILE* file;
		/// �t�H���_�[�Ŏw�肵���t�@�C�����J��
		if (fopen_s(&file, openFileName.lpstrFile, "rb") == 0)
		{
			_stageInfo.targetData.clear();
			std::vector<vec_target>().swap(_stageInfo.targetData);

			for (int i = 0; i < _stageInfo.scores.size(); ++i)
			{
				fread(&_stageInfo.scores[i], sizeof(int), 1, file);
				fread((char*)_stageInfo.names[i].c_str(), (sizeof(char) * 3), 1, file);
			}
			/// �Q�[���̉�ʃT�C�Y����G�f�B�^�[�̉�ʃT�C�Y�̔{�������߂Ă���
			Vector2<double> rate = Vector2<double>((double)_screen.x / _gameScreen.x,
												   (double)_screen.y / _gameScreen.y);

			TargetData target;
			std::vector<TargetData> targetData;

			int waveCnt, targetCnt;
			/// �E�F�[�u���̓ǂݍ���
			fread(&waveCnt, sizeof(int), 1, file);

			for (int w = 0; w < waveCnt; ++w)
			{
				/// �^�[�Q�b�g���̓ǂݍ���
				fread(&targetCnt, sizeof(int), 1, file);
				targetData.resize(targetCnt);
				for (int t = 0; t < targetCnt; ++t)
				{
					fread(&target.type,		  sizeof(unsigned char), 1, file);
					fread(&target.dispTime,   sizeof(unsigned int), 1, file);
					fread(&target.banishTime, sizeof(unsigned int), 1, file);
					fread(&target.pos.x, sizeof(int), 1, file);
					target.pos.x *= rate.x;
					fread(&target.pos.y, sizeof(int), 1, file);
					target.pos.y *= rate.y;

					targetData[t] = target;
				}
				_stageInfo.targetData.push_back(targetData);
				targetData.clear();
			}
			std::vector<TargetData>().swap(targetData);
			fclose(file);
		}
	}
	return true;
}

/// �G�f�B�^�[�̍X�V�p
void Editer::Update()
{
	while (ProcessMessage() == 0 && !_input->IsTrigger(KEY_INPUT_ESCAPE))
	{
		ClsDrawScreen();
		_input->Update();
		(this->*_nowMode)();
		(this->*_drawer)();

		ScreenFlip();
	}
}