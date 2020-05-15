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
	// ウェーブ数が0の時、ウェーブ数を1にする
	_waveCnt = (_waveCnt == 0 ? 1 : _waveCnt);
	_stageData.reserve(_waveCnt);
	_stageData.resize(_waveCnt);

	_targetCnt = 3;
}

void Stage::Edit()
{
	/// 設定する的の初期化
	_nowMode	 = &Stage::EditUpdate;
	_targetState = std::make_unique<TargetType>();

	/// 現在のウェーブ数の初期化
	_nowWaveCnt = _nowTargetCnt = 0;

	/// ウェーブごとのターゲット数の設定
	for (auto& wave : _stageData)
	{
		wave.reserve(_targetCnt);
		wave.resize(_targetCnt);

		for (auto& target : wave)
		{
			/// 的情報の初期化
			target.type			= 0;
			target.appearTime	= 60;
			target.dispTime		= 60;
			target.pos.x		= _screen.x;
			target.pos.y		= _screen.y;
		}
	}

	/// 文字サイズの初期化
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
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "現在のウェーブ数", strlen("現在のウェーブ数"));
	DrawString((_screen.x / 2) - (strWidth / 2), (_screen.y / 2) - strHeight, "現在のウェーブ数", 0xffffff);

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
		/// 的数の減算
		_targetCnt = (_targetCnt > 3 ? _targetCnt - 1 : 3);
	}
	else if (_input->IsTrigger(KEY_INPUT_UP))
	{
		/// 的数の加算
		_targetCnt = (_targetCnt < _targetCntMax ? _targetCnt + 1 : _targetCntMax);
	}
	else {}

	int strWidth, strHeight;
	strWidth = strHeight = 0;

	SetFontSize(80);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "出現する的の数", strlen("出現する的の数"));
	DrawString((_screen.x / 2) - (strWidth / 2), (_screen.y / 2) - strHeight, "出現する的の数", 0xffffff);

	SetFontSize(140);
	DrawFormatString((_screen.x / 2), (_screen.y / 2) + strHeight, 0x88ff88, "%d", _targetCnt);
}

void Stage::EditUpdate()
{
	/// ステージデータの初期化
	if (IsReset())
	{
		Wave();
		_stageData.clear();
		return;
	}

	/// ロード処理
	if (IsLoad())
	{
		Load();
	}

	/// セーブ処理
	if (IsSave)
	{
		Save();
	}
	
	/// ターゲット情報の更新
	_targetState->Update(_nowWaveCnt, _nowTargetCnt, _input, _stageData);
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
	char fileSize[MAX_PATH] = "";											// ファイル名のサイズと最後に\0を入れる
	ZeroMemory(&openFileName, sizeof(openFileName));						// 構造体の初期化
	openFileName.lStructSize = sizeof(OPENFILENAME);						// 構造体の大きさ
	openFileName.lpstrFilter = TEXT("binファイル(*.bin)\0*.bin\0\0");		// 形式の選択
	openFileName.lpstrFile   = fileSize;									// 開くファイル名の長さ
	openFileName.lpstrInitialDir = ("../");									// 開くフォルダの指定
	openFileName.nMaxFile	 = MAX_PATH;									// 開くファイルの大きさ
	openFileName.lpstrDefExt = (".bin");									// 保存するときのファイル形式

	if (GetSaveFileName(&openFileName) == true)
	{
		FILE* file;
		/// フォルダーで指定したファイルを開く
		if (fopen_s(&file, openFileName.lpstrFile, "wb") == 0)
		{
			/// エディターの画面サイズからゲームの画面サイズの倍率を求めている
			Vector2<double> rate = Vector2<double>((double)_gameScreen.x / _screen.x,
												   (double)_gameScreen.y / _screen.y);

			for (auto wave : _stageData)
			{
				for (auto target : wave)
				{
					/// ターゲット情報を書き込む
					fwrite(&target.type, sizeof(target.type), 1, file);
					fwrite(&target.dispTime, sizeof(target.dispTime), 1, file);
					fwrite(&target.appearTime, sizeof(target.appearTime), 1, file);
					target.pos.x *= rate.x;
					fwrite(&target.pos.x, sizeof(target.pos.x), 1, file);
					target.pos.y *= rate.y;
					fwrite(&target.pos.y, sizeof(target.pos.y), 1, file);
				}
				/// ウェーブ情報のエンドポイントを書き込む
				fwrite(&_waveEnd, sizeof(_waveEnd), 1, file);
			}
			/// eofの書き込み
			char eof = -1;
			fwrite(&eof, sizeof(eof), 1, file);
			fclose(file);
		}
		else
		{
			MessageBox(GetMainWindowHandle(),
				"ファイルが見つかりませんでした。",
				"Not Found File",
				MB_OK);
		}
	}
	return true;
}

bool Stage::Load()
{
	// ファイルフォルダーを開いて読み込むための初期化
	char fileSize[MAX_PATH]	 = "";											// ファイル名のサイズと最後に\0を入れる
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
		_stageData.clear();
		FILE* file;
		/// フォルダーで指定したファイルを開く
		if (fopen_s(&file, openFileName.lpstrFile, "rb") == 0)
		{
			/// ゲームの画面サイズからエディターの画面サイズの倍率を求めている
			Vector2<double> rate = Vector2<double>((double)_gameScreen.x / _screen.x,
				(double)_gameScreen.y / _screen.y);

			TargetData target;
			std::vector<TargetData> targetData;
			int bytePos = 0;
			char checkVal = 0;

			fseek(file, bytePos, SEEK_SET);
			while (checkVal != -1)
			{
				/// 読み込むバイト番地を指定する
				fseek(file, bytePos, SEEK_SET);

				{/* 的情報の読み込み */

					/// 的の種別ID
					fread(&target.type, sizeof(target.type), 1, file);
					bytePos += sizeof(target.type);

					/// 的の出現する時間
					fread(&target.dispTime, sizeof(target.dispTime), 1, file);
					bytePos += sizeof(target.dispTime);

					/// 的が出現してから消えるまでの時間
					fread(&target.appearTime, sizeof(target.appearTime), 1, file);
					bytePos += sizeof(target.appearTime);

					/// 的のX座標
					fread(&target.pos.x, sizeof(target.pos.x), 1, file);
					target.pos.x *= rate.x;
					bytePos += sizeof(target.pos.x);

					/// 的のY座標
					fread(&target.pos.y, sizeof(target.pos.y), 1, file);
					target.pos.y *= rate.y;
					bytePos += sizeof(target.pos.y);
				}

				/// 的情報の登録
				targetData.push_back(target);

				/// ウェーブ数の末尾かの確認を行う
				fread(&checkVal, sizeof(checkVal), 1, file);
				if (checkVal == _waveEnd)
				{
					/// 1ウェーブで出現する的情報の登録
					bytePos += sizeof(checkVal);
					_stageData.push_back(targetData);
					targetData.clear();
				}

				/// eofの確認を行うための読み込み
				fread(&checkVal, sizeof(checkVal), 1, file);
			}
			fclose(file);
		}
		else
		{
			MessageBox(GetMainWindowHandle(),
				"ファイルが見つかりませんでした。",
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
	/// ステージエディタで必要な情報を描画する予定
}
