#pragma once

#include <memory>
#include <vector>
#include <windows.h>
#include "Geometry.h"

class Input;
class TargetState;

enum class TargetID
{
	NORMAL,			// 通常の的
	SPECIAL,		// 特別な的
	DEDUCTION,		// 減点される的
	MAX
};

// 的の情報
struct TargetData
{
	u_char type;			// 的の種類		(1番目のデータ)
	u_int appearTime;		// 出現する時間 (2番目のデータ)
	u_int dispTime;			// 表示する時間 (3番目のデータ)
	Vector2<int> pos;		// 座標			(4番目のデータ)
};

using vec2_target  = std::vector<std::vector<TargetData>>;

using unique_input = std::unique_ptr<Input>;

// 的の状態遷移用ポインター
using unique_state = std::unique_ptr<TargetState>;

class Stage
{
public:
	static Stage& Instance()
	{
		/// シングルトンの中身が空の時に生成する
		if (!s_Instance)
		{
			s_Instance.reset(new Stage());
		}
		return *s_Instance;
	}
	void Update();

	// 設定する的情報の変更用
	void ChagneState(TargetState* targetState);
	
	// エディターの画面サイズ取得用
	Vector2<int> GetScreenSize() const;
private:
	Stage();
	~Stage();

	struct EditerDeleter
	{
		void operator()(Stage* pointer)
		{
			delete pointer;
		}
	};

	static std::unique_ptr<Stage, EditerDeleter> s_Instance;

	bool Init();

	// ウェーブ数の設定用
	void Wave();

	void Target();

	// ステージの編集
	void Edit();

	void WaveUpdate();
	void TargetUpdate();
	void EditUpdate();

	bool Save();
	bool Load();

	// ステージデータを初期化するかの確認用
	bool IsReset();

	// 読み込みを行うかの確認用
	bool IsSave();

	// 書き込みを行うかの確認用
	bool IsLoad();

	int _waveCnt;						// ウェーブ数のカウント保存用
	int _configTarget;					// 設定中のターゲット
	std::vector<int> _waveTargetCnt;	// 1ウェーブ中のターゲット数

	// ステージ設定で使用する情報
	int _nowWaveCnt, _nowTargetCnt;

	// ウェーブのデータ保持用変数
	vec2_target _stageData;

	// 入力関係のポインター
	unique_input _input;

	unique_state _targetState;

	// エディットモードの関数ポインター
	void (Stage::* _nowMode)();

	const Vector2<int> _gameScreen;	// ゲームの画面サイズ
	const Vector2<int> _screen;		// エディターの画面サイズ
	const int _targetCntMax;		// 的の最大数

	const char _waveEnd;			// バイナリ上でウェーブの終了を確認するためのもの
};