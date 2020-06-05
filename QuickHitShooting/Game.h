#pragma once
#include <memory>
#include "Scene/Scene.h"
#include "Geometry.h"
#undef PlaySound;

class FileSystem;
class SoundPlayer;
class Peripheral;

// ゲーム全体を制御するクラス
class Game
{
private:
	Game();							// 生成禁止
	Game(const Game&);				// コピー禁止
	void operator=(const Game&);	// 代入禁止

	const Vector2<int> _screenSize;
	std::unique_ptr<Peripheral> _peripheral;
	std::shared_ptr<FileSystem> _fileSystem;
	std::shared_ptr<SoundPlayer> _soundPlayer;

public:
	static Game& Instance()
	{
		static Game instance;
		return instance;
	}
	~Game();

	// 初期化系
	// 基本素材のロードとか初期処理を行う
	void Initialize();

	// 中にループあり
	// これを呼ぶとゲームが始まる
	void Run();

	// ライブラリの後処理など
	void Terminate();

	const std::shared_ptr<FileSystem> GetFileSystem()const;
	const std::shared_ptr<SoundPlayer> GetSoundPlayer()const;

	const Vector2<int>& GetScreenSize()const;
};
