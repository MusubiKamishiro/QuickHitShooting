#pragma once
#include "../Geometry.h"
#include "../Game.h"
#include "../Loader/FileSystem.h"
#include "../Loader/ImageLoader.h"
#include "../Loader/SoundLoader.h"
#include "../SoundPlayer.h"

class Enemy
{
protected:
	virtual void HitSound() = 0;
	Vector2<int> _pos;
	Rect _rect;

	int _targetImg;
	int _dispTime;
	int _banishTime;

	bool _isAlive;		// true : ����, false : ���S
public:
	virtual ~Enemy();

	void Update(); 
	void Draw();

	bool HitShot();

	bool Destroy();

	virtual int GetScore() const = 0;
	Vector2<int> GetPos() const;
	Rect GetRect() const;
};

