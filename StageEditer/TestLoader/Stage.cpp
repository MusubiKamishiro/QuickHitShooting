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

	/// ウェーブ数を取得している
	_waveTargetCnt.clear();
	_waveTargetCnt.reserve(_waveCnt);
	_waveTargetCnt.resize(_waveCnt);

	/// 1ウェーブごとの的数の初期化
	for (auto& tCnt : _waveTargetCnt)
	{
		tCnt = 3;
	}
}

void Stage::Edit()
{
	/// 設定する的の初期化
	_nowMode	 = &Stage::EditUpdate;
	_targetState = std::make_unique<TargetType>();

	/// ウェーブの生成
	_stageInfo.targetData.reserve(_waveCnt);
	_stageInfo.targetData.resize(_waveCnt);

	/// スコアデータの初期化
	for (int i = 0; i < _stageInfo.scores.size(); ++i)
	{
		_stageInfo.scores[i] = 0;
		_stageInfo.names[i]  = "AAA";
	}

	/// ステージデータの初期化
	auto wCnt = _stageInfo.targetData.begin();
	for (; wCnt != _stageInfo.targetData.end(); ++wCnt)
	{
		/// 1ウェーブごとの的数の設定
		auto cnt = wCnt - _stageInfo.targetData.begin();
		(*wCnt).reserve(_waveTargetCnt[cnt]);
		(*wCnt).resize(_waveTargetCnt[cnt]);

		/// 的情報の初期化
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

	/// エディターで使用する値の初期化
	_nowWaveCnt = _nowTargetCnt = 0;
}

void Stage::WaveUpdate()
{
	/// ターゲットモードに移行する
	if (_input->IsTrigger(KEY_INPUT_SPACE))
	{
		Target();
		return;
	}

	if (_input->IsTrigger(KEY_INPUT_UP))
	{
		/// ウェーブ数の減算
		_waveCnt = (_waveCnt > 1 ? _waveCnt - 1 : 1);
	}
	else if (_input->IsTrigger(KEY_INPUT_DOWN))
	{
		/// ウェーブ数の加算
		++_waveCnt;
	}
	else {}

	Vector2<int> strSize;
	std::string text;
	SetFontSize(80);
	text = "現在のウェーブ数";
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) - strSize.y, text.c_str(), 0xffffff);

	SetFontSize(140);
	text = std::to_string(_waveCnt);
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) + (strSize.y / 2), text.c_str(), 0x88ff88);
}

void Stage::TargetUpdate()
{
	/// ウェーブモードに移行する
	if (_input->IsTrigger(KEY_INPUT_F1))
	{
		Wave();
		return;
	}

	/// エディットモードに移行する
	if (_input->IsTrigger(KEY_INPUT_SPACE))
	{
		Edit();
		return;
	}

	if (_input->IsTrigger(KEY_INPUT_LEFT))
	{
		/// 的数の減算
		_waveTargetCnt[_configTarget] = (_waveTargetCnt[_configTarget] > 3 ? _waveTargetCnt[_configTarget] - 1 : 3);
	}
	else if (_input->IsTrigger(KEY_INPUT_RIGHT))
	{
		/// 的数の加算
		_waveTargetCnt[_configTarget] = (_waveTargetCnt[_configTarget] < _targetCntMax ? _waveTargetCnt[_configTarget] + 1 : _targetCntMax);
	}
	else {}

	/// 設定する的の切り替え
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
	text = "出現する的の数";
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) - strSize.y, text.c_str(), 0xffffff);

	/// 現在設定している的の表示
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

	/// 出現する的数の設定
	SetFontSize(140);
	text = std::to_string(_waveTargetCnt[_configTarget]);
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) + (strSize.y / 2), text.c_str(), 0x88ff88);
}

void Stage::EditUpdate()
{
	/// ターゲット情報の更新
	_targetState->Update(_nowWaveCnt, _nowTargetCnt, _input, _stageInfo.targetData);
	/// ステージデータの初期化
	if (IsReset())
	{
		Wave();
		_stageInfo.targetData.clear();
		return;
	}

	/// ロード処理
	if (IsLoad())
	{
		Load();
	}

	/// セーブ処理
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
			"ステージ情報を最初から設定しますか？",
			"ステージデータの初期化",
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
			"ステージデータをセーブしますか？",
			"セーブの確認",
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
			"ステージデータをロードしますか？",
			"ロードの確認",
			MB_OKCANCEL) == IDOK)
		{
			return true;
		}
	}
	return false;
}

bool Stage::Save()
{
	// ファイルフォルダーを開いて書き込むための初期化
	OPENFILENAME openFileName;
	char fileSize[MAX_PATH] = "";											// ファイル名のサイズと最後に\0を入れる
	ZeroMemory(&openFileName, sizeof(openFileName));						// 構造体の初期化
	openFileName.lStructSize = sizeof(OPENFILENAME);						// 構造体の大きさ
	openFileName.lpstrFilter = TEXT("binファイル(*.bin)\0*.bin\0\0");		// 形式の選択
	openFileName.lpstrFile	 = fileSize;									// 開くファイル名の長さ
	openFileName.lpstrInitialDir = ("../");									// 開くフォルダの指定
	openFileName.nMaxFile	 = MAX_PATH;									// 開くファイルの大きさ
	openFileName.lpstrDefExt = (".bin");									// 保存するときのファイル形式

	if (GetSaveFileName(&openFileName) == true)
	{
		FILE* file;
		/// フォルダーで指定したファイルを開く
		if (fopen_s(&file, openFileName.lpstrFile, "wb") == 0)
		{
			_stageInfo.targetData.clear();
			/// ステージデータの書き込み
			for (int i = 0; i < _stageInfo.scores.size(); ++i)
			{
				fwrite(&_stageInfo.scores[i], sizeof(int), 1, file);
				fwrite(&_stageInfo.names[i], (sizeof(char) * 3), 1, file);
			}
			/// エディターの画面サイズからゲームの画面サイズの倍率を求めている
			Vector2<double> rate = Vector2<double>((double)_gameScreen.x / _screen.x,
												   (double)_gameScreen.y / _screen.y);

			Vector2<int> registPos;
			int targetCnt = 0;

			/// 書き込むウェーブ数の設定
			int waveCnt = _stageInfo.targetData.size();
			fwrite(&waveCnt, sizeof(int), 1, file);

			for (int w = 0; w < waveCnt; ++w)
			{
				/// 書き込むターゲット数の設定
				targetCnt = _stageInfo.targetData[w].size();
				fwrite(&targetCnt, sizeof(int), 1, file);
				for (int t = 0; t < targetCnt; ++t)
				{
					/// ターゲットデータの書き込み
					fwrite(&_stageInfo.targetData[w][t].type,		 sizeof(unsigned char), 1, file);
					fwrite(&_stageInfo.targetData[w][t].dispTime,	 sizeof(unsigned int), 1, file);
					fwrite(&_stageInfo.targetData[w][t].appearTime,  sizeof(unsigned int), 1, file);
					registPos.x = (_stageInfo.targetData[w][t].pos.x * rate.x);
					fwrite(&registPos.x, sizeof(int), 1, file);
					registPos.y = (_stageInfo.targetData[w][t].pos.y * rate.x);
					fwrite(&registPos.y, sizeof(int), 1, file);
				}
			}
			/// ファイルを閉じる
			fclose(file);
		}
	}
	return true;
}

bool Stage::Load()
{
	// ファイルフォルダーを開いて読み込むための初期化
	OPENFILENAME openFileName;
	char fileSize[MAX_PATH] = "";											// ファイル名のサイズと最後に\0を入れる
	ZeroMemory(&openFileName, sizeof(openFileName));						// 構造体の初期化
	openFileName.lStructSize = sizeof(OPENFILENAME);						// 構造体の大きさ
	openFileName.lpstrFilter = TEXT("binファイル(*.bin)\0*.bin\0\0");		// 形式の選択
	openFileName.lpstrFile	 = fileSize;									// 開くファイル名の長さ
	openFileName.lpstrInitialDir = ("../");									// 開くフォルダの指定
	openFileName.nMaxFile	 = MAX_PATH;									// 開くファイルの大きさ
	openFileName.lpstrDefExt = (".bin");									// 保存するときのファイル形式

	if (GetSaveFileName(&openFileName) == true)
	{
		// とりあえずデータの初期化を行う
		FILE* file;
		/// フォルダーで指定したファイルを開く
		if (fopen_s(&file, openFileName.lpstrFile, "rb") == 0)
		{
			_stageInfo.targetData.clear();
			/// スコアデータの読み込み
			for (int i = 0; i < _stageInfo.scores.size(); ++i)
			{
				fread(&_stageInfo.scores[i], sizeof(int), 1, file);
				fread((char*)_stageInfo.names[i].c_str(), (sizeof(char) * 3), 1, file);
			}

			/// ゲームの画面サイズからエディターの画面サイズの倍率を求めている
			Vector2<double> rate = Vector2<double>((double)_screen.x / _gameScreen.x,
												   (double)_screen.y / _gameScreen.y);

			TargetData target;
			std::vector<TargetData> targetData;

			int waveCnt, targetCnt;
			/// ウェーブ数の読み込み
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