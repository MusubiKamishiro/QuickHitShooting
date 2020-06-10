#pragma once
#include <array>
#include "../Geometry.h"
#include "../Game.h"
#include "../Loader/FileSystem.h"
#include "../Loader/ImageLoader.h"
#include "../Loader/SoundLoader.h"
#include "../SoundPlayer.h"

// •ªŠ„‚µ‚½‰æ‘œ‚Ìî•ñ
struct imgElem
{
	Vector2<float> pos;
	Vector2<float> vel;
	int deg;
};

class Enemy
{
protected:
	void InitImgElem();
	virtual void HitSound() = 0;
	Vector2<int> _pos;
	std::array<imgElem, 25> _imgElements;	// 
	Rect _rect;

	int _targetImg;
	int _dispTime;
	int _banishTime;

	bool _isAlive;		// true : ¶‘¶, false : €–S

	const Size _eSize = Size(75, 75);	// “G‚Ì‘å‚«‚³
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

