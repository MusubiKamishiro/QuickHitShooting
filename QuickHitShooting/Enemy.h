#pragma once
#include "Geometry.h"

class Enemy
{
private:

protected:
	Vector2<int> _pos;
	Rect _rect;

	int _dispTime;
	int _appearTime;

	bool _isAlive;		// true : ¶‘¶, false : €–S
public:
	virtual ~Enemy();

	virtual void Draw() = 0;
	void Update(); 

	// ’e‚ª“–‚½‚Á‚½‚Ìˆ—
	void HitShot();

	// “G‚Ìíœ—p
	bool Destroy();

	virtual int GetScore() const = 0;
	Vector2<int> GetPos() const;
	Rect GetRect() const;
};

