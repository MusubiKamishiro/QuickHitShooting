#pragma once
#include <memory>
#include "Scene/Scene.h"
#include "Geometry.h"
#undef PlaySound;

class FileSystem;
class SoundPlayer;
class Peripheral;

// �Q�[���S�̂𐧌䂷��N���X
class Game
{
private:
	Game();							// �����֎~
	Game(const Game&);				// �R�s�[�֎~
	void operator=(const Game&);	// ����֎~

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

	// �������n
	// ��{�f�ނ̃��[�h�Ƃ������������s��
	void Initialize();

	// ���Ƀ��[�v����
	// ������ĂԂƃQ�[�����n�܂�
	void Run();

	// ���C�u�����̌㏈���Ȃ�
	void Terminate();

	const std::shared_ptr<FileSystem> GetFileSystem()const;
	const std::shared_ptr<SoundPlayer> GetSoundPlayer()const;

	const Vector2<int>& GetScreenSize()const;
};
