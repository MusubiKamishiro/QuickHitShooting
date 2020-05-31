#pragma once

#include <memory>
#include <vector>
#include <array>
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
	unsigned char type;			// 的の種類		(1番目のデータ)
	unsigned int appearTime;	// 出現する時間 (2番目のデータ)
	unsigned int dispTime;		// 表示する時間 (3番目のデータ)
	Vector2<int> pos;			// 座標			(4番目のデータ)
};

using vec_target = std::vector<TargetData>;

using unique_input = std::unique_ptr<Input>;

// 的設定の状態遷移用ポインター
using unique_state = std::unique_ptr<TargetState>;

struct StageInfo
{
	// ウェーブのデータ保持用変数
	std::vector<vec_target> targetData;
	std::array<int, 3> scores;
	std::array<std::string, 3> names;
};

class Editer
{
public:
	static Editer& Instance()
	{
		/// シングルトンの中身が空の時に生成する
		if (!s_Instance)
		{
			s_Instance.reset(new Editer());
		}
		return *s_Instance;
	}
	void Update();

	// 設定する的情報の変更用
	void ChagneState(TargetState* targetState);
	
	// エディターの画面サイズ取得用
	Vector2<int> GetScreenSize() const;
private:
	Editer();
	~Editer();

	/// シングルトンのデリーター
	struct EditerDeleter
	{
		void operator()(Editer* pointer)
		{
			delete pointer;
		}
	};

	static std::unique_ptr<Editer, EditerDeleter> s_Instance;

	bool Init();

	/// ステージ設定の初期化用
	void Wave();
	void Target();
	void Stage();

	/// ステージ設定の更新用
	void WaveUpdate();
	void TargetUpdate();
	void StageUpdate();

	/// ステージデータの描画用
	void WaveDrawer();
	void TargetDrawer();
	void StageDrawer();

	/// ステージデータの保存と読み込み用
	bool Save();
	bool Load();

	/// 特定の行動を行うかの判定用
	bool IsReset();
	bool IsSave();
	bool IsLoad();

	int _waveCnt;						// ウェーブ数のカウント保存用
	int _configTarget;					// 設定中のターゲット
	std::vector<int> _waveTargetCnt;	// 1ウェーブ中のターゲット数

	// ステージ設定で使用する情報
	int _nowWaveCnt, _nowTargetCnt;

	// 入力関係のポインター
	unique_input _input;

	unique_state _targetState;

	StageInfo _stageInfo;

	// エディットモードの関数ポインター
	void (Editer::* _nowMode)();
	void (Editer::* _drawer)();

	const Vector2<int> _gameScreen;	// ゲームの画面サイズ
	const Vector2<int> _screen;		// エディターの画面サイズ
	const int _targetCntMax;		// 的の最大数

	const char _waveEnd;			// バイナリ上でウェーブの終了を確認するためのもの
};