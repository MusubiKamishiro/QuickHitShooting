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
#include "../Menu.h"
#include "../TrimString.h"

#include "../Gun.h"
#include "../Enemy.h"
#include "../NormalEnemy.h"
#include "../SpecialEnemy.h"
#include "../DeductionEnemy.h"
#include "../CollisionDetector.h"

GamePlayingScene::GamePlayingScene(const GunStatus& gunState, const StageData& stageData)
{
	_pal = 0;
	
	_updater = &GamePlayingScene::FadeinUpdate;
	_drawer  = &GamePlayingScene::TestDraw;

	_gun.reset(new Gun(gunState));
	_cd.reset(new CollisionDetector());
	_menu.reset(new Menu());
	_trimString.reset(new TrimString());

	/// ステージの読み込み
	_stageData = stageData;

	ImageData data;

	/// ゲーム中の背景画像の取得
	Game::Instance().GetFileSystem()->Load("img/game.png", data);
	_gameBg = data.GetHandle();

	Game::Instance().GetFileSystem()->Load("img/pause.png", data);
	int i = data.GetHandle();
	_menu->AddMenuList("pause", Vector2<int>(_scrSize.x - 50, 0), Vector2<int>(_scrSize.x, 50), i);
	_menu->AddMenuList("test", Vector2<int>(0, 0), Vector2<int>(50, 50), i);

	_hitFlag = false;

	_hitCount = 0.0f;
	_shotCount = 0.0f;

	_waveCnt = 0;
	_score = 0;
	
	/// 敵の仮生成
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
		ResultData r;
		r.score = _score;
		r.hitRate = ((_hitCount / _shotCount) * 100);
		r.ranking[0] = std::make_pair("ムスビ", 765283);
		r.ranking[1] = std::make_pair("miyabi", 346315);
		r.ranking[2] = std::make_pair("りばー", 72);

		SceneManager::Instance().ChangeScene(std::make_unique<ResultScene>(r));
	}
	else
	{
		_pal -= 20;
	}
}

void GamePlayingScene::WaitUpdate(const Peripheral& p)
{
	//ポーズボタンを押したらポーズシーンに切り替え
	if (_menu->CheckClick("pause", p))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}
	else if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		if (_gun->Shot())
		{
			++_shotCount;

			Vector2<int> pos = p.GetMousePos();
			for (auto enemy : _enemies)
			{
				if (_cd->IsCollision(pos, enemy->GetRect()))
				{
					enemy->HitShot();
					_hitFlag = true;
					++_hitCount;
					_score += enemy->GetScore();
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
	/// 背景の描画
	DxLib::DrawGraph(0, 0, _gameBg, true);
	_trimString->ChangeFontSize(40);
	DxLib::DrawFormatString(_trimString->GetStringCenterPosx("00000"), 0, 0x000000, "%05d", _score);
	DxLib::DrawFormatString(0, 0, 0x000000, "WAVE %d", (_waveCnt + 1));

	for (auto& enemy : _enemies)
	{
		enemy->Draw();
	}

	_gun->Draw();
	_menu->Draw();

	if (_hitFlag)
	{
		DxLib::DrawString(500, 0, "Hit", 0xff0000);
	}
}

bool GamePlayingScene::CreateEnemy()
{
	/// 仮でステージデータを読み込んでいる

	if (_waveCnt < (int)_stageData.GetStageData().targetData.size())
	{
		auto data = _stageData.GetStageData().targetData[_waveCnt];
		for (auto target : data)
		{
			/// 敵の生成
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
		/// 通常の的を生成する
		return std::make_shared<NormalEnemy>(target.dispTime, target.appearTime, target.pos);
	}
	else if (target.type == 1)
	{
		/// 特別な的を生成する
		return std::make_shared<SpecialEnemy>(target.dispTime, target.appearTime, target.pos);
	}
	else if (target.type == 2)
	{
		/// 減点の的を生成する
		return std::make_shared<DeductionEnemy>(target.dispTime, target.appearTime, target.pos);
	}
	else{}

	/// 該当なし
	return nullptr;
}

void GamePlayingScene::Update(const Peripheral& p)
{
	//
	if (_menu->CheckClick("test", p))
	{
		_updater = &GamePlayingScene::FadeoutUpdate;
	}
	//

	for (auto& enemy : _enemies)
	{
		enemy->Update();
	}

	/// 削除できる敵を検索している
	auto result = std::remove_if(_enemies.begin(),
								 _enemies.end(),
								 [](std::shared_ptr<Enemy>& enemy) { return enemy->Destroy(); });

	/// 敵の削除
	_enemies.erase(result, _enemies.end());

	if (_enemies.size() <= 0)
	{
		++_waveCnt;
		/// 1ウェーブが終了した時、すぐに次のウェーブに出現する的の用意をしている。
		if (!CreateEnemy())
		{
			/// 全てのウェーブを終えた時に入る処理
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

	// フェードイン,アウトのための幕
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);
}
