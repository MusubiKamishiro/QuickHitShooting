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
#include "../SoundPlayer.h"
#include "../Menu.h"
#include "../TrimString.h"

#include "../Gun.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/NormalEnemy.h"
#include "../Enemy/SpecialEnemy.h"
#include "../Enemy/DeductionEnemy.h"
#include "../CollisionDetector.h"

#include "EffekseerForDXLib.h"

/// セレクトシーンからステージを選択したときに入る
GamePlayingScene::GamePlayingScene(const GunStatus& gunState, const std::string& stagePath)
{
	_updater = &GamePlayingScene::FadeinUpdate;
	_drawer  = &GamePlayingScene::CountDownDraw;

	_gun.reset(new Gun(gunState));
	_cd.reset(new CollisionDetector());
	_menu.reset(new Menu());
	_trimString.reset(new TrimString());

	StageData stage;
	Game::Instance().GetFileSystem()->Load(stagePath.c_str(), stage);
	/// ステージの読み込み
	_stageData = stage;

	ImageData data;

	/// 下地画像の取得
	Game::Instance().GetFileSystem()->Load("img/plate/bulletBd.png", data);
	_bulletBd = data.GetHandle();

	Game::Instance().GetFileSystem()->Load("img/plate/waveBd.png", data);
	_waveBd = data.GetHandle();

	/// 背景画像の取得
	Game::Instance().GetFileSystem()->Load("img/game.png", data);
	_gameBg = data.GetHandle();

	Game::Instance().GetFileSystem()->Load("img/button/menu.png", data);
	int i = data.GetHandle();
	_menu->AddMenuList("pause", Vector2<int>(_scrSize.x - 50, 0), Vector2<int>(_scrSize.x, 50), i);

	/// 音の読み込みと登録
	SoundData sdata;
	Game::Instance().GetFileSystem()->Load("sound/bgm/game.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("gameBGM", sdata.GetHandle(), 50);

	Game::Instance().GetFileSystem()->Load("sound/se/countdown.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("countdown", sdata.GetHandle(), 55);

	Game::Instance().GetFileSystem()->Load("sound/se/start.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("start", sdata.GetHandle(), 70);

	Game::Instance().GetFileSystem()->Load("sound/se/finish.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("finish", sdata.GetHandle(), 60);

	Game::Instance().GetFileSystem()->Load("sound/se/pause.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("pause", sdata.GetHandle(), 70);

	Game::Instance().GetFileSystem()->Load("sound/se/reload.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("reload", sdata.GetHandle(), 70);

	/// プレイヤー情報の初期化
	_hitFlag  = false;
	_hitCount = _shotCount = 0.0f;
	_waveCnt  = _score = _pal = 0;

	_waitCnt  = 240;
}

/// ポーズシーンからリトライを選択したときに入る
GamePlayingScene::GamePlayingScene(const std::shared_ptr<Gun>& gun, const std::string& stagePath)
{
	_updater = &GamePlayingScene::FadeinUpdate;
	_drawer  = &GamePlayingScene::CountDownDraw;

	_gun = gun;
	_cd.reset(new CollisionDetector());
	_menu.reset(new Menu());
	_trimString.reset(new TrimString());

	StageData stage;
	Game::Instance().GetFileSystem()->Load(stagePath.c_str(), stage);
	/// ステージの読み込み
	_stageData = stage;

	ImageData data;

	/// 下地画像の取得
	Game::Instance().GetFileSystem()->Load("img/plate/bulletBd.png", data);
	_bulletBd = data.GetHandle();

	Game::Instance().GetFileSystem()->Load("img/plate/waveBd.png", data);
	_waveBd = data.GetHandle();

	/// 背景画像の取得
	Game::Instance().GetFileSystem()->Load("img/game.png", data);
	_gameBg = data.GetHandle();

	Game::Instance().GetFileSystem()->Load("img/button/menu.png", data);
	int i = data.GetHandle();
	_menu->AddMenuList("pause", Vector2<int>(_scrSize.x - 50, 0), Vector2<int>(_scrSize.x, 50), i);

	/// 音の読み込みと登録
	SoundData sdata;
	Game::Instance().GetFileSystem()->Load("sound/bgm/game.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("gameBGM", sdata.GetHandle(), 50);

	Game::Instance().GetFileSystem()->Load("sound/se/countdown.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("countdown", sdata.GetHandle(), 55);

	Game::Instance().GetFileSystem()->Load("sound/se/start.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("start", sdata.GetHandle(), 70);

	Game::Instance().GetFileSystem()->Load("sound/se/finish.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("finish", sdata.GetHandle(), 60);

	Game::Instance().GetFileSystem()->Load("sound/se/pause.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("pause", sdata.GetHandle(), 70);

	Game::Instance().GetFileSystem()->Load("sound/se/reload.mp3", sdata);
	Game::Instance().GetSoundPlayer()->AddSound("reload", sdata.GetHandle(), 70);

	/// プレイヤー情報の初期化
	_hitFlag = false;
	_hitCount = _shotCount = 0.0f;
	_waveCnt = _score = _pal = 0;

	_waitCnt = 240;
}

GamePlayingScene::~GamePlayingScene()
{
}

void GamePlayingScene::FadeinUpdate(const Peripheral & p)
{
	Game::Instance().GetSoundPlayer()->PlaySound("gameBGM", true);

	if (_pal > 255)
	{
		_pal = 255;
		_updater = &GamePlayingScene::CountDownUpdate;
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
		r.score	  = _score;
		r.hitRate = ((_hitCount / _shotCount) * 100);

		/// ランキングデータの書き込み
		for (int i = 0; i < r.ranking.size(); ++i)
		{
			r.ranking[i] = std::make_pair(_stageData.GetStageData().names[i], _stageData.GetStageData().scores[i]);
		}
		r.name = _stageData.GetStageData().stageName;
		SceneManager::Instance().ChangeScene(std::make_unique<ResultScene>(_gun, r));
	}
	else
	{
		_pal -= 20;
	}
}

/// カウントダウンの更新用
void GamePlayingScene::CountDownUpdate(const Peripheral& p)
{
	if (_waitCnt == 60)
	{
		Game::Instance().GetSoundPlayer()->PlaySound("start");
	}
	if (_waitCnt <= 0)
	{
		CreateEnemy();
		_pal	 = 255;
		_updater = &GamePlayingScene::WaitUpdate;
		_drawer  = &GamePlayingScene::GameDraw;
	}
	if (!(_waitCnt % 60) && _waitCnt >= 61)
	{
		Game::Instance().GetSoundPlayer()->PlaySound("countdown");
	}
	--_waitCnt;
}

/// ゲーム終了時の更新
void GamePlayingScene::FinishUpdate(const Peripheral& p)
{
	if (_waitCnt <= 0)
	{
		_updater = &GamePlayingScene::FadeoutUpdate;
	}
	--_waitCnt;
}

/// ゲーム中の更新用1
void GamePlayingScene::WaitUpdate(const Peripheral& p)
{
	if (_enemies.size() <= 0)
	{
		++_waveCnt;
		/// 1ウェーブが終了した時、すぐに次のウェーブに出現する的の用意をしている。
		if (!CreateEnemy())
		{
			/// 全てのウェーブを終えた時に入る処理
			_updater = &GamePlayingScene::FinishUpdate;
			_drawer  = &GamePlayingScene::FinishDraw;

			Game::Instance().GetSoundPlayer()->StopSound("gameBGM");
			Game::Instance().GetSoundPlayer()->PlaySound("finish");

			_waitCnt = 240;
			return;
		}
	}

	//ポーズボタンを押したらポーズシーンに切り替え
	if (_menu->CheckClick("pause", p))
	{
		Game::Instance().GetSoundPlayer()->PlaySound("pause");
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>(_gun, _stageData.GetStageData().stageName));
	}
	else if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		if (_gun->Shot())
		{
			++_shotCount;
			Vector2<int> pos = p.GetMousePos();
			for (auto enemy : _enemies)
			{
				if (_cd->IsCollision(pos, enemy->GetRect()) &&
					enemy->HitShot())
				{
					++_hitCount;	
					_score  += enemy->GetScore();
					_hitFlag = true;
				}
			}

			/// エフェクトの再生を行う
			if (p.IsTrigger(MOUSE_INPUT_LEFT))
			{
				/// エフェクトの再生
				_playEffect = PlayEffekseer2DEffect(_shotEffect);

				_efkPos = p.GetMousePos();
			}
		}

		
	}
	else if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		Game::Instance().GetSoundPlayer()->PlaySound("reload");
		_gun->Reload();
	}
}

/// カウントダウン時の描画
void GamePlayingScene::CountDownDraw()
{
	/// 背景の描画
	DxLib::DrawGraph(0, 0, _gameBg, true);

	DxLib::DrawGraph(0, -23, _waveBd, true);
	DxLib::DrawGraph(570, _scrSize.y - 97, _bulletBd, true);

	Vector2<int> _strSize;
	if ((_waitCnt - 1) / 60 >= 1)
	{
		/// カウントダウン時の描画
		_trimString->ChangeFontSize(180);
		std::string text = std::to_string((_waitCnt - 1) / 60);
		GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, text.c_str(), strlen(text.c_str()));

		DxLib::DrawString((_scrSize.x / 2) - (_strSize.x / 2), (_scrSize.y / 2) - (_strSize.y / 2), 
						   text.c_str(), 0xff0000);
	}
	else
	{
		/// 開始の合図
		_trimString->ChangeFontSize(100);
		GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, "START", strlen("START"));
		DxLib::DrawString((_scrSize.x / 2) - (_strSize.x / 2), (_scrSize.y / 2) - (_strSize.y / 2),
						  "START", 0xff0000);
	}
}

/// ゲーム中の描画
void GamePlayingScene::GameDraw()
{
	/// 背景の描画
	DxLib::DrawGraph(0, 0, _gameBg, true);

	DxLib::DrawGraph(0, -23, _waveBd, true);
	DxLib::DrawGraph(570, _scrSize.y - 97, _bulletBd, true);

	_trimString->ChangeFontSize(40);
	DxLib::DrawFormatString(_trimString->GetStringCenterPosx("00000"), 0, 0x000000, "%05d", _score);
	DxLib::DrawFormatString(100, 0, 0x000000, "WAVE %d", (_waveCnt + 1));

	for (auto& enemy : _enemies)
	{
		enemy->Draw();
	}
	_gun->Draw();
	_menu->Draw();
}

/// ゲーム終了時の描画
void GamePlayingScene::FinishDraw()
{
	/// 背景の描画
	DxLib::DrawGraph(0, 0, _gameBg, true);

	DxLib::DrawGraph(0, -23, _waveBd, true);
	DxLib::DrawGraph(570, _scrSize.y - 97, _bulletBd, true);

	/// 開始の合図
	Vector2<int> _strSize;
	_trimString->ChangeFontSize(100);
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, "FINISH", strlen("FINISH"));
	DxLib::DrawString((_scrSize.x / 2) - (_strSize.x / 2), (_scrSize.y / 2) - (_strSize.y / 2),
					  "FINISH", 0xff0000);
}

/// 敵の生成
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

/// 敵の情報取得用
std::shared_ptr<Enemy> GamePlayingScene::GetEnemyInfo(const TargetData& target)
{
	if (target.type == 0)
	{
		/// 通常の的を生成する
		return std::make_shared<NormalEnemy>(target.dispTime, target.banishTime, target.pos);
	}
	else if (target.type == 1)
	{
		/// 特別な的を生成する
		return std::make_shared<SpecialEnemy>(target.dispTime, target.banishTime, target.pos);
	}
	else if (target.type == 2)
	{
		/// 減点の的を生成する
		return std::make_shared<DeductionEnemy>(target.dispTime, target.banishTime, target.pos);
	}
	else{}
	/// 該当なし
	return nullptr;
}

/// 更新処理
void GamePlayingScene::Update(const Peripheral& p)
{
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

	(this->*_updater)(p);
}

/// 描画処理
void GamePlayingScene::Draw()
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0xffffff, true);
	
	(this->*_drawer)();

	DrawEffect();

	// フェードイン,アウトのための幕
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(_pal - 255));
	DxLib::DrawBox(0, 0, _scrSize.x, _scrSize.y, 0x000000, true);
}
