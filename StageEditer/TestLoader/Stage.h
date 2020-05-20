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

	Vector2<int> pos;
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

	void ChagneState(TargetState* targetState);
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

	bool IsReset();
	bool IsSave();
	bool IsLoad();

	void Draw();

	// ウェーブ数のカウント保存用
	int _waveCnt;
	std::vector<int> _targetCnts;
	int _targetCnt;

	int _nowWaveCnt, _nowTargetCnt;

	// ウェーブのデータ保持用変数
	vec2_target _stageData;

	// 入力関係のポインター
	unique_input _input;

	unique_state _targetState;

	// エディットモードの関数ポインター
	void (Stage::* _nowMode)();

	static std::unique_ptr<Stage, EditerDeleter> s_Instance;

	const Vector2<int> _gameScreen;
	const Vector2<int> _screen;
	const int _targetCntMax;	// 的の最大数(現状、5個)

	const char _waveEnd;
};