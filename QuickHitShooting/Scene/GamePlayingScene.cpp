#include <DxLib.h>
#include <algorithm>
#include "GamePlayingScene.h"
#include "ResultScene.h"
#include "SceneManager.h"
#include "PauseScene.h"
#include "../Peripheral.h"

#include "../Game.h"
#include "../Loader/FileSystem.h"
#include "../Loader/ImageLoader.h"
#include "../Loader/SoundLoader.h"
#include "../Loader/StageLoader.h"
#include "../Menu.h"
#include "../TrimString.h"

#include "../Gun.h"
#include "../Enemy.h"
#include "../NormalEnemy.h"
#include "../SpecialEnemy.h"
#include "../DeductionEnemy.h"
#include "../CollisionDetector.h"

GamePlayingScene::GamePlayingScene(const GunStatus& gunState)
{
	_pal = 0;
	
	_updater = &GamePlayingScene::FadeinUpdate;
	_drawer  = &GamePlayingScene::TestDraw;

	_gun.reset(new Gun(gunState));
	_cd.reset(new CollisionDetector());
	_menu.reset(new Menu());
	_trimString.reset(new TrimString());

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/pause.png", data);
	int i = data.GetHandle();
	_menu->AddMenuList("pause", Vector2<int>(_scrSize.x - 50, 0), Vector2<int>(_scrSize.x, 50), i);

	hitFlag = false;

	_waveCnt = 0;
	_score = 0;
	/// �X�e�[�W�ǂݍ���(������Z���N�g�V�[���Ɉړ�����\��)

	auto stageCnt = []()
	{
		int cnt = 0;
		HANDLE handle;
		WIN32_FIND_DATA findData;
		std::string searchName = "../StageData/*.bin";
		handle = FindFirstFile(searchName.c_str(), &findData);

		do {
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				cnt++;
			}
		} while (FindNextFile(handle, &findData));
		FindClose(handle);

		if (cnt == 0)
		{
			MessageBox(GetMainWindowHandle(),
				"�X�e�[�W�f�[�^��������܂���ł����B",
				"Not Found StageData",
				MB_OK);
		}
		return cnt;
	};

	int cnt = stageCnt();

	/// �S�X�e�[�W��ǂݍ���(���̏���)
	StageData stage;
	for (int i = 0; i < cnt; ++i)
	{
		TargetData debug;
		std::string stageNum = std::to_string(i + 1);
		Game::Instance().GetFileSystem()->Load("StageData/stage" + stageNum + ".bin", stage);

		for (auto wave : stage.GetStageData())
		{
			for (auto target : wave)
			{
				debug = target;
			}
		}
	}

	/// �G�̉�����
	CreateEnemy();
}

GamePlayingScene::~GamePlayingScene()
{
}

void GamePlayingScene::FadeinUpdate(const Peripheral & p)
{
	if (_pal > 255)
	{
		_pal = 255;
		_updater = &GamePlayingScene::WaitUpdate;
	}
	else
	{
		_pal += 20;
	}
}

void GamePlayingScene::FadeoutUpdate(const Peripheral & p)
{
	if (_pal <= 0)
	{
		SceneManager::Instance().ChangeScene(std::make_unique<ResultScene>(_score));
	}
	else
	{
		_pal -= 20;
	}
}

void GamePlayingScene::WaitUpdate(const Peripheral& p)
{
	//�|�[�Y�{�^������������|�[�Y�V�[���ɐ؂�ւ�
	if (_menu->CheckClick("pause", p))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}
	else if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		if (_gun->Shot())
		{
			Vector2<int> pos = p.GetMousePos();
			for (auto enemy : _enemies)
			{
				if (_cd->IsCollision(pos, enemy->GetRect()))
				{
					enemy->HitShot();
					hitFlag = true;
				}
			}
		}
	}
	else if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		_gun->Reload();
	}
}

void GamePlayingScene::TestDraw()
{
	_trimString->ChangeFontSize(40);
	DxLib::DrawFormatString(_trimString->GetStringCenterPosx("00000"), 0, 0x000000, "%05d", _score);
	DxLib::DrawFormatString(0, 0, 0x000000, "WAVE %d", (_waveCnt + 1));

	for (auto& enemy : _enemies)
	{
		enemy->Draw();
	}

	_gun->Draw();
	_menu->Draw();

	if (hitFlag)
	{
		DxLib::DrawString(500, 0, "Hit", 0xff0000);
	}
}

bool GamePlayingScene::CreateEnemy()
{
	/// ���ŃX�e�[�W�f�[�^��ǂݍ���ł���
	StageData stage;
	Game::Instance().GetFileSystem()->Load("StageData/stage1.bin", stage);

	if (_waveCnt < stage.GetStageData().size())
	{
		auto data = stage.GetStageData()[_waveCnt];
		for (auto target : data)
		{
			/// �G�̐���
			_enemies.push_back(GetEnemyInfo(target));
		}
		return true;
	}
	return false;
}

std::shared_ptr<Enemy> GamePlayingScene::GetEnemyInfo(const TargetData& target)
{
	if (target.type == 0)
	{
		/// �ʏ�̓I�𐶐�����
		return std::make_shared<NormalEnemy>(target.dispTime, target.appearTime, target.pos);
	}
	else if (target.type == 1)
	{
		/// ���ʂȓI�𐶐�����
		return std::make_shared<SpecialEnemy>(target.dispTime, target.appearTime, target.pos);
	}
	else if (target.type == 2)
	{
		/// ���_�̓I�𐶐�����
		return std::make_shared<DeductionEnemy>(target.dispTime, target.appearTime, target.pos);
	}
	else{}

	/// �Y���Ȃ�
	return nullptr;
}

void GamePlayingScene::Update(const Peripheral& p)
{
	for (auto& enemy : _enemies)
	{
		enemy->Update();
	}

	/// �폜�ł���G���������Ă���
	auto result = std::remove_if(_enemies.begin(),
								 _enemies.end(),
								 [](std::shared_ptr<Enemy>& enemy) { return enemy->Destroy(); });

	/// �G�̍폜
	_enemies.erase(result, _enemies.end());

	if (_enemies.size() <= 0)
	{
		++_waveCnt;
		/// 1�E�F�[�u���I���������A�����Ɏ��̃E�F�[�u�ɏo������I�̗p�ӂ����Ă���B
		if (!CreateEnemy())
		{
			/// �S�ẴE�F�[�u���I�������ɓ��鏈��
			_updater = &GamePlayingScene::FadeoutUpdate;
		}
	}

	(this->*_updater)(p);
}

void GamePlayingScene::Draw()
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0xffffff, true);

	(this->*_drawer)();

	// �t�F�[�h�C��,�A�E�g�̂��߂̖�
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);
}
