#include <DxLib.h>
#include <random>
#include "SelectScene.h"
#include "GamePlayingScene.h"
#include "SceneManager.h"
#include "../Peripheral.h"
#include "../TrimString.h"

#include "../Game.h"
#include "../Loader/FileSystem.h"
#include "../Loader/ImageLoader.h"
#include "../Menu.h"

SelectScene::SelectScene() : _dightMax(6)
{
	_pal = 0;
	_trimString = std::make_unique<TrimString>();

	/// �X�N���[���T�C�Y�̎擾
	Vector2<int> btnSize = Vector2<int>(300, 150);

	/// ���n�摜�̎擾
	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/plate/rankBd.png", data);
	_rankBd = data.GetHandle();

	Game::Instance().GetFileSystem()->Load("img/plate/gunBd.png", data);
	_gunBd = data.GetHandle();

	Game::Instance().GetFileSystem()->Load("img/plate/stageBd.png", data);
	_stageBd = data.GetHandle();

	/// �w�i�摜�̎擾
	Game::Instance().GetFileSystem()->Load("img/select.png", data);
	_selectBg = data.GetHandle();

	/// �e�̃��j���[�o�^
	int space = 95;
	Game::Instance().GetFileSystem()->Load("img/gun1.png", data);
	int img = data.GetHandle();

	_menu.reset(new Menu());
	_gunState.name		 = "Gun1";
	_gunState.maxBullets = 700;
	_gunState.maxBulletsInMagazine = 10;
	AddGunMenu(_gunState, Vector2<int>(space, _scrSize.y - btnSize.y - 30),
						  Vector2<int>(space + btnSize.x, _scrSize.y - 30), img);

	Game::Instance().GetFileSystem()->Load("img/gun2.png", data);
	img = data.GetHandle();

	_gunState.name		 = "Gun2";
	_gunState.maxBullets = 500;
	_gunState.maxBulletsInMagazine = 10;
	AddGunMenu(_gunState, Vector2<int>((_scrSize.x / 2) - (btnSize.x / 2), _scrSize.y - btnSize.y - 30),
						  Vector2<int>((_scrSize.x / 2) + (btnSize.x / 2), _scrSize.y - 30), img);

	Game::Instance().GetFileSystem()->Load("img/gun3.png", data);
	img = data.GetHandle();

	_gunState.name		 = "Gun3";
	_gunState.maxBullets = 300;
	_gunState.maxBulletsInMagazine = 40;
	AddGunMenu(_gunState, Vector2<int>(_scrSize.x - space - btnSize.x, _scrSize.y - btnSize.y - 30),
						  Vector2<int>(_scrSize.x - space, _scrSize.y - 30), img);

	btnSize = Vector2<int>(150, 150);
	/// �����{�^���̕\��
	Game::Instance().GetFileSystem()->Load("img/leftArrow.png", data);
	img = data.GetHandle();
	_menu->AddMenuList("left", Vector2<int>((btnSize.x / 2), (_scrSize.y / 2) - (btnSize.y / 2)),
							   Vector2<int>(btnSize.x + (btnSize.x / 2), (_scrSize.y / 2) + (btnSize.y / 2)), img);

	/// �E���{�^���̕\��
	Game::Instance().GetFileSystem()->Load("img/rightArrow.png", data);
	img = data.GetHandle();
	_menu->AddMenuList("right", Vector2<int>(_scrSize.x - (btnSize.x / 2),(_scrSize.y / 2) + (btnSize.y / 2)), 
								Vector2<int>(_scrSize.x - btnSize.x - (btnSize.x / 2), (_scrSize.y / 2) - (btnSize.y / 2)), img);

	Game::Instance().GetFileSystem()->Load("img/stageboard.png", data);

	StageInit();

	_updater = &SelectScene::FadeinUpdate;
}

SelectScene::~SelectScene()
{
}

void SelectScene::StageInit()
{
	/// �t�H���_�[�̒��ɂ���X�e�[�W�����擾�������
	auto stageCnt = []()
	{
		/// ���ׂ�X�e�[�W�n���h���̎擾
		HANDLE handle;
		WIN32_FIND_DATA findData;
		std::string searchName = "StageData/*.bin";
		handle = FindFirstFile(searchName.c_str(), &findData);
		int cnt = 0;

		do {
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				/// �X�e�[�W�������Z������
				cnt++;
			}
		} while (FindNextFile(handle, &findData));

		/// �J�����t�H���_�[�����
		FindClose(handle);

		/// �X�e�[�W�f�[�^���ǂݍ��߂Ȃ��������ɓ���
		if (cnt == 0)
		{
			MessageBox(GetMainWindowHandle(),
				"�X�e�[�W�f�[�^��������܂���ł����B",
				"Not Found StageData",
				MB_OK);
		}
		return cnt;
	};
	/// �X�e�[�W�J�E���g�̏�����
	_stageCnt = 0;
	_stageCntMax = stageCnt();

	/// �S�X�e�[�W�̓ǂݍ��݂��s��
	StageData stage;
	_stageDatas.resize(_stageCntMax);
	for (int i = 0; i < _stageCntMax; ++i)
	{
		std::string stagePath = GetStagePath(i + 1);
		Game::Instance().GetFileSystem()->Load(stagePath.c_str(), stage);

		/// �X�e�[�W�f�[�^�̓o�^
		_stageDatas[i] = stage;
	}
}

StageData SelectScene::GetStageData(const int& num)
{
	std::string stagePath = GetStagePath(num + 1);
	StageData stage;
	Game::Instance().GetFileSystem()->Load(stagePath.c_str(), stage);

	return stage;
}

void SelectScene::FadeinUpdate(const Peripheral& p)
{
	if (_pal >= 255)
	{
		_pal = 255;
		_updater = &SelectScene::WaitUpdate;
	}
	else
	{
		_pal += 20;
	}
}

void SelectScene::FadeoutUpdate(const Peripheral& p)
{
	if (_pal <= 0)
	{
		SceneManager::Instance().ChangeScene(std::make_unique<GamePlayingScene>(_gunState, _stageDatas[_stageCnt]));
	}
	else
	{
		_pal -= 20;
	}
}

void SelectScene::WaitUpdate(const Peripheral& p)
{
	for (int i = 0; i < _gunStatus.size(); ++i)
	{
		if (_menu->CheckClick(_gunStatus[i].name.c_str(), p))
		{
			_gunState = _gunStatus[i];
			_updater  = &SelectScene::FadeoutUpdate;
		}
	}

	/// �X�e�[�W�I�����s�����߂̃��j���[
	if (_menu->CheckClick("right", p))
	{
		_stageCnt = (_stageCnt + 1) % _stageCntMax;
	}
	else if (_menu->CheckClick("left", p))
	{
		_stageCnt = ((_stageCnt + _stageCntMax) - 1) % _stageCntMax;
	}
	else{}

	_menu->Update(p);
}

std::string SelectScene::GetStagePath(const int& num) const
{
	return "StageData/stage" + std::to_string(num) + ".bin";
}

void SelectScene::AddGunMenu(const GunStatus& gunstate, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const int& img)
{
	_menu->AddMenuList(gunstate.name.c_str(), ltPos, rbPos, img);
	_gunStatus.push_back(gunstate);
}

void SelectScene::Update(const Peripheral& p)
{
	(this->*_updater)(p);
}

void SelectScene::Draw()
{
	/// 6���\���̃X�R�A�̕�������擾�������
	auto GetScoreDight = [](const int& score, const int& max)
	{
		int divNum = 1;
		int divCnt = 0;
		std::string name;

		/// �X�R�A�̌��������߂Ă���
		while ((score / divNum) >= 1)
		{
			++divCnt;
			divNum *= 10;
		}

		/// �X�R�A��6���ȉ��̎��A0�Ŗ��߂鏈��
		if (divCnt < max)
		{
			for (int i = 0; i < (max - divCnt); ++i)
			{
				name += "0";
			}
		}
		if (score > 0)
		{
			name += std::to_string(score);
		}
		
		return name;
	};

	/* �`��J�n�ʒu */

	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0xffffff, true);

	/// �w�i�̕`��
	DxLib::DrawGraph(0, 0, _selectBg, true);

	/// ���n�̕`��
	DxLib::DrawGraph(40, 0, _rankBd, true);
	DxLib::DrawGraph(300, _scrSize.y / 2 - 110, _stageBd, true);
	DxLib::DrawGraph(40, _scrSize.y - 230, _gunBd, true);

	/// ���j���[�̕`��
	_menu->Draw();

	_trimString->ChangeFontSize(70);
	std::string text;
	Vector2<int> strSize;

	/// �X�R�A�̊Ԋu(debug�p)
	int space = 400;
	
	/// �X�R�A�̕`��
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, "000000", strlen("000000"));
	auto score = _stageDatas[_stageCnt].GetStageData().scores;
	for (int i = 0; i < score.size(); ++i)
	{
		text = GetScoreDight(score[i], _dightMax);

		DrawString(240 + (space * i) - (strSize.x / 2), 150 - strSize.y / 2,
				  text.c_str(), 0x000000);
	}

	/// �v���C���[�����L���O�̕`��
	text = std::to_string(0) + "�� " + "AAA";
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	auto name = _stageDatas[_stageCnt].GetStageData().names;
	for (int i = 0; i < name.size(); ++i)
	{
		text = std::to_string(i + 1) + "�� " + name[i];
		DrawString(240 + (space * i) - (strSize.x / 2), 80 - (strSize.y / 2),
			text.c_str(), 0xff0000);
	}

	/// �X�e�[�W�̕`��
	_trimString->ChangeFontSize(120);
	text = "STAGE" + std::to_string(_stageCnt + 1);

	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((Game::Instance().GetScreenSize().x / 2) - (strSize.x / 2), 
			   (Game::Instance().GetScreenSize().y / 2) - (strSize.y / 2) + (strSize.y / 7),
			   text.c_str(), 0x000080);
	
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);
}
