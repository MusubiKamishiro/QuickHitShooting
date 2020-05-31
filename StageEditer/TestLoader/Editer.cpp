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

/// 画面ウィンドウなどの初期化
bool Editer::Init()
{
	///	ウィンドウの初期化
	ChangeWindowMode(true);
	SetGraphMode(_screen.x, _screen.y, 32);
	if (DxLib::DxLib_Init() == -1)
	{
		return false;
	}
	SetMainWindowText("StageEditer");
	SetDrawScreen(DX_SCREEN_BACK);

	/// ステージエディターの初期状態
	Wave();

	return false;
}

/// 設定する的情報の変更用
void Editer::ChagneState(TargetState* targetState)
{
	_targetState.reset(targetState);
}

/// 画面サイズ取得用
Vector2<int> Editer::GetScreenSize() const
{
	return _screen;
}

/// ウェーブ設定の初期化
void Editer::Wave()
{
	_nowMode = &Editer::WaveUpdate;
	_drawer  = &Editer::WaveDrawer;
	_waveCnt = 1;

	/// ステージデータの初期化
	_stageInfo.targetData.clear();
	std::vector<vec_target>().swap(_stageInfo.targetData);
}

/// ターゲット設定の初期化
void Editer::Target()
{
	_nowMode = &Editer::TargetUpdate;
	_drawer  = &Editer::TargetDrawer;

	/// ウェーブ数の初期化
	_waveTargetCnt.clear();
	std::vector<int>().swap(_waveTargetCnt);

	/// ウェーブ数の設定を行う
	_waveTargetCnt.reserve(_waveCnt);
	_waveTargetCnt.resize(_waveCnt);

	/// 1ウェーブごとの的数の初期化
	for (auto& tCnt : _waveTargetCnt)
	{
		tCnt = 3;
	}
}

/// ステージ設定の初期化
void Editer::Stage()
{
	/// 設定する的の初期化
	_nowMode	 = &Editer::StageUpdate;
	_drawer		 = &Editer::StageDrawer;
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
	for (unsigned int i = 0; i < _stageInfo.targetData.size(); ++i)
	{
		/// 1ウェーブごとの的数の設定
		_stageInfo.targetData[i].reserve(_waveTargetCnt[i]);
		_stageInfo.targetData[i].resize(_waveTargetCnt[i]);

		/// 的情報の初期化
		for (auto& target : _stageInfo.targetData[i])
		{
			target.type		  = 0;
			target.banishTime = 60;
			target.dispTime	  = 60;
			target.pos.x	  = _screen.x / 2;
			target.pos.y	  = _screen.y / 2;
		}
	}

	/// エディターで使用する値の初期化
	_nowWaveCnt = _nowTargetCnt = 0;
}

/// ウェーブ設定の更新用
void Editer::WaveUpdate()
{
	/// 的設定に移行する
	if (_input->IsTrigger(KEY_INPUT_SPACE))
	{
		Target();
		return;
	}

	/// ウェーブ数の設定
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

/// 的設定の更新用
void Editer::TargetUpdate()
{
	/// ウェーブ設定に移行する
	if (_input->IsTrigger(KEY_INPUT_F1))
	{
		Wave();
		return;
	}

	/// ステージデータ設定に移行する
	if (_input->IsTrigger(KEY_INPUT_SPACE))
	{
		Stage();
		return;
	}

	/// 的数の設定
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

	/// 設定する的の切替
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

/// ステージ設定の更新用
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

/// ウェーブ設定の描画
void Editer::WaveDrawer()
{
	Vector2<int> strSize;
	std::string text;
	SetFontSize(80);
	text = "Now Wave Count";
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) - strSize.y, text.c_str(), 0xffffff);

	/// 現在のウェーブ数
	SetFontSize(140);
	text = std::to_string(_waveCnt);
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) + (strSize.y / 2), text.c_str(), 0x88ff88);
}

/// 的設定の描画
void Editer::TargetDrawer()
{
	Vector2<int> strSize;
	std::string text;
	SetFontSize(80);
	text = "Appear Target Count";
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) - strSize.y, text.c_str(), 0xffffff);

	/// 設定を行っている的の表示
	int nowTargetColor;
	for (int i = 0; i < _waveTargetCnt.size(); ++i)
	{
		nowTargetColor = (_configTarget == i ? 0xffff00 : 0xffffff);
		text = std::to_string(i + 1) + " : " + std::to_string(_waveTargetCnt[i]);
		GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
		DrawString(_screen.x - strSize.x, strSize.y * i, text.c_str(), nowTargetColor);
	}

	/// 現在の的数
	SetFontSize(140);
	text = std::to_string(_waveTargetCnt[_configTarget]);
	GetDrawStringSize(&strSize.x, &strSize.y, nullptr, text.c_str(), strlen(text.c_str()));
	DrawString((_screen.x / 2) - (strSize.x / 2), (_screen.y / 2) + (strSize.y / 2), text.c_str(), 0x88ff88);
}

/// ステージ設定の描画
void Editer::StageDrawer()
{
	_targetState->Draw(_nowWaveCnt, _nowTargetCnt, _stageInfo.targetData);
}

/// ステージの再設定を行うかの判定用
bool Editer::IsReset()
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

/// ステージデータの保存を行うかの判定用
bool Editer::IsSave()
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

/// ステージデータを保存行うかの判定用
bool Editer::IsLoad()
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

/// セーブ処理
bool Editer::Save()
{
	/// ファイルフォルダーを開いて書き込むための初期化
	OPENFILENAME openFileName;
	char fileSize[MAX_PATH] = "";											/// ファイル名のサイズと最後に\0を入れる
	ZeroMemory(&openFileName, sizeof(openFileName));						/// 構造体の初期化
	openFileName.lStructSize = sizeof(OPENFILENAME);						/// 構造体の大きさ
	openFileName.lpstrFilter = TEXT("binファイル(*.bin)\0*.bin\0\0");		/// 形式の選択(バイナリ形式)
	openFileName.lpstrFile	 = fileSize;									/// 開くファイル名の長さ
	openFileName.lpstrInitialDir = ("../");									/// 開くフォルダの指定
	openFileName.nMaxFile	 = MAX_PATH;									/// 開くファイルサイズ
	openFileName.lpstrDefExt = (".bin");									/// 保存するときのファイル形式

	if (GetSaveFileName(&openFileName) == true)
	{
		FILE* file;
		/// フォルダーで指定したファイルを開く
		if (fopen_s(&file, openFileName.lpstrFile, "wb") == 0)
		{
			char name[3];
			for (int i = 0; i < _stageInfo.scores.size(); ++i)
			{
				fwrite(&_stageInfo.scores[i], sizeof(int), 1, file);
				for (int c = 0; c < sizeof(name) / sizeof(name[0]); ++c)
				{
					/// 文字の取得を行っている
					name[c] = _stageInfo.names[i][c];
				}
				fwrite(name, (sizeof(char) * 3), 1, file);
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
					fwrite(&_stageInfo.targetData[w][t].banishTime,  sizeof(unsigned int), 1, file);
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

/// ロード処理
bool Editer::Load()
{
	/// ファイルフォルダーを開いて書き込むための初期化
	OPENFILENAME openFileName;
	char fileSize[MAX_PATH] = "";											/// ファイル名のサイズと最後に\0を入れる
	ZeroMemory(&openFileName, sizeof(openFileName));						/// 構造体の初期化
	openFileName.lStructSize = sizeof(OPENFILENAME);						/// 構造体の大きさ
	openFileName.lpstrFilter = TEXT("binファイル(*.bin)\0*.bin\0\0");		/// 形式の選択(バイナリ形式)
	openFileName.lpstrFile = fileSize;									/// 開くファイル名の長さ
	openFileName.lpstrInitialDir = ("../");									/// 開くフォルダの指定
	openFileName.nMaxFile = MAX_PATH;									/// 開くファイルサイズ
	openFileName.lpstrDefExt = (".bin");									/// 保存するときのファイル形式

	if (GetSaveFileName(&openFileName) == true)
	{
		FILE* file;
		/// フォルダーで指定したファイルを開く
		if (fopen_s(&file, openFileName.lpstrFile, "rb") == 0)
		{
			_stageInfo.targetData.clear();
			std::vector<vec_target>().swap(_stageInfo.targetData);

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
				/// ターゲット数の読み込み
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

/// エディターの更新用
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