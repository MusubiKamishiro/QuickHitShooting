#pragma once
#include "../Geometry.h"
#include "../Game.h"
#include "../Loader/FileSystem.h"
#include "../Loader/ImageLoader.h"

class Enemy
{
protected:
	Vector2<int> _pos;
	Rect _rect;

	int _targetImg;
	int _dispTime;
	int _banishTime;

	bool _isAlive;		// true : ¶‘¶, false : €–S
public:
	virtual ~Enemy();

	virtual void Draw() = 0;
	void Update(); 

	// ’e‚ª“–‚½‚Á‚½‚Ìˆ—
	bool HitShot();

	// “G‚Ìíœ—p
	bool Destroy();

	virtual int GetScore() const = 0;
	Vector2<int> GetPos() const;
	Rect GetRect() const;
};

