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

	_stageInfo.targetData.clear();
}

void Stage::Target()
{
	_nowMode = &Stage::TargetUpdate;

	/// �E�F�[�u�����擾���Ă���
	_waveTargetCnt.clear();
	_waveTargetCnt.reserve(_waveCnt);
	_waveTargetCnt.resize(_waveCnt);

	/// 1�E�F�[�u���Ƃ̓I���̏�����
	for (auto& tCnt : _waveTargetCnt)
	{
		tCnt = 3;
	}
}

void Stage::Edit()
{
	/// �ݒ肷��I�̏�����
	_nowMode	 = &Stage::EditUpdate;
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
	auto wCnt = _stageInfo.targetData.begin();
	for (; wCnt != _stageInfo.targetData.end(); ++wCnt)
	{
		/// 1�E�F�[�u���Ƃ̓I���̐ݒ�
		auto cnt = wCnt - _stageInfo.targetData.begin();
		(*wCnt).reserve(_waveTargetCnt[cnt]);
		(*wCnt).resize(_waveTargetCnt[cnt]);

		/// �I���̏�����
		for (auto& target : (*wCnt))
		{
			target.type		  = 0;
			target.appearTime = 60;
			target.dispTime	  = 60;
			target.pos.x	  = _screen.x / 2;
			target.pos.y	  = _screen.y / 2;
		}
	}
	_waveTargetCnt.clear();

	/// �G�f�B�^�[�Ŏg�p����l�̏�����
	_nowWaveCnt = _nowTargetCnt = 0;
}

void Stage::WaveUpdate()
{
	/// �^�[�Q�b�g���[�h�Ɉڍs����
	if (_input->IsTrigger(KEY_INPUT_SPACE))
	{
		Target();
		return;
	}

	if (_input->IsTrigger(KEY_INPUT_UP))
	{
		/// �E�F�[�u���̌��Z
		_waveCnt = (_waveCnt > 1 ? _waveCnt - 1 : 1);
	}
	else if (_input->IsTrigger(KEY_INPUT_DOWN))
	{
		/// �E�F�[�u���̉��Z
		++_waveCnt;
	}
	else {}

	Vector2<int> strSize;
	std::string text;
	SetFontSize(80);
	text = "���݂̃E�F�[�u��";
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) - strSize.y, text.c_str(), 0xffffff);

	SetFontSize(140);
	text = std::to_string(_waveCnt);
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) + (strSize.y / 2), text.c_str(), 0x88ff88);
}

void Stage::TargetUpdate()
{
	/// �E�F�[�u���[�h�Ɉڍs����
	if (_input->IsTrigger(KEY_INPUT_F1))
	{
		Wave();
		return;
	}

	/// �G�f�B�b�g���[�h�Ɉڍs����
	if (_input->IsTrigger(KEY_INPUT_SPACE))
	{
		Edit();
		return;
	}

	if (_input->IsTrigger(KEY_INPUT_LEFT))
	{
		/// �I���̌��Z
		_waveTargetCnt[_configTarget] = (_waveTargetCnt[_configTarget] > 3 ? _waveTargetCnt[_configTarget] - 1 : 3);
	}
	else if (_input->IsTrigger(KEY_INPUT_RIGHT))
	{
		/// �I���̉��Z
		_waveTargetCnt[_configTarget] = (_waveTargetCnt[_configTarget] < _targetCntMax ? _waveTargetCnt[_configTarget] + 1 : _targetCntMax);
	}
	else {}

	/// �ݒ肷��I�̐؂�ւ�
	if (_input->IsTrigger(KEY_INPUT_UP))
	{
		int targetMax = _waveTargetCnt.size();
		_configTarget = ((_configTarget - 1) + targetMax) % targetMax;
	}
	else if (_input->IsTrigger(KEY_INPUT_DOWN))
	{
		int targetMax = _waveTargetCnt.size();
		_configTarget = (_configTarget + 1) % targetMax;
	}
	else{}

	Vector2<int> strSize;
	std::string text;
	SetFontSize(80);
	text = "�o������I�̐�";
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) - strSize.y, text.c_str(), 0xffffff);

	/// ���ݐݒ肵�Ă���I�̕\��
	int nowTargetColor;
	auto tCnt = _waveTargetCnt.begin();
	for (; tCnt != _waveTargetCnt.end(); ++tCnt)
	{
		auto cnt = tCnt - _waveTargetCnt.begin();
		nowTargetColor = (_configTarget == cnt ? 0xffff00 : 0xffffff);
		text = std::to_string(cnt + 1) + " : " + std::to_string((*tCnt));
		GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
		DrawString(_screen.x - strSize.x, strSize.y * cnt, text.c_str(), nowTargetColor);
	}

	/// �o������I���̐ݒ�
	SetFontSize(140);
	text = std::to_string(_waveTargetCnt[_configTarget]);
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) + (strSize.y / 2), text.c_str(), 0x88ff88);
}

void Stage::EditUpdate()
{
	/// �^�[�Q�b�g���̍X�V
	_targetState->Update(_nowWaveCnt, _nowTargetCnt, _input, _stageInfo.targetData);
	/// �X�e�[�W�f�[�^�̏�����
	if (IsReset())
	{
		Wave();
		_stageInfo.targetData.clear();
		return;
	}

	/// ���[�h����
	if (IsLoad())
	{
		Load();
	}

	/// �Z�[�u����
	if (IsSave())
	{
		Save();
	}
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
	OPENFILENAME openFileName;
	char fileSize[MAX_PATH] = "";											// �t�@�C�����̃T�C�Y�ƍŌ��\0������
	ZeroMemory(&openFileName, sizeof(openFileName));						// �\���̂̏�����
	openFileName.lStructSize = sizeof(OPENFILENAME);						// �\���̂̑傫��
	openFileName.lpstrFilter = TEXT("bin�t�@�C��(*.bin)\0*.bin\0\0");		// �`���̑I��
	openFileName.lpstrFile	 = fileSize;									// �J���t�@�C�����̒���
	openFileName.lpstrInitialDir = ("../");									// �J���t�H���_�̎w��
	openFileName.nMaxFile	 = MAX_PATH;									// �J���t�@�C���̑傫��
	openFileName.lpstrDefExt = (".bin");									// �ۑ�����Ƃ��̃t�@�C���`��

	if (GetSaveFileName(&openFileName) == true)
	{
		FILE* file;
		/// �t�H���_�[�Ŏw�肵���t�@�C�����J��
		if (fopen_s(&file, openFileName.lpstrFile, "wb") == 0)
		{
			_stageInfo.targetData.clear();
			/// �X�e�[�W�f�[�^�̏�������
			for (int i = 0; i < _stageInfo.scores.size(); ++i)
			{
				fwrite(&_stageInfo.scores[i], sizeof(int), 1, file);
				fwrite(&_stageInfo.names[i], (sizeof(char) * 3), 1, file);
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
					fwrite(&_stageInfo.targetData[w][t].appearTime,  sizeof(unsigned int), 1, file);
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

bool Stage::Load()
{
	// �t�@�C���t�H���_�[���J���ēǂݍ��ނ��߂̏�����
	OPENFILENAME openFileName;
	char fileSize[MAX_PATH] = "";											// �t�@�C�����̃T�C�Y�ƍŌ��\0������
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
		FILE* file;
		/// �t�H���_�[�Ŏw�肵���t�@�C�����J��
		if (fopen_s(&file, openFileName.lpstrFile, "rb") == 0)
		{
			_stageInfo.targetData.clear();
			/// �X�R�A�f�[�^�̓ǂݍ���
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
				fread(&targetCnt, sizeof(int), 1, file);
				targetData.resize(targetCnt);
				for (int t = 0; t < targetCnt; ++t)
				{
					fread(&target.type,		  sizeof(unsigned char), 1, file);
					fread(&target.dispTime,   sizeof(unsigned int), 1, file);
					fread(&target.appearTime, sizeof(unsigned int), 1, file);
					fread(&target.pos.x, sizeof(int), 1, file);
					target.pos.x *= rate.x;
					fread(&target.pos.y, sizeof(int), 1, file);
					target.pos.y *= rate.y;

					targetData[t] = target;
				}
				_stageInfo.targetData.push_back(targetData);
				targetData.clear();
				std::vector<TargetData>().swap(targetData);
			}

			fclose(file);
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

		ScreenFlip();
	}
}

void Stage::ChagneState(TargetState* targetState)
{
	_targetState.reset(targetState);
}

Vector2<int> Stage::GetScreenSize() const
{
	return _screen;
}