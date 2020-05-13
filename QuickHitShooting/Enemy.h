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

public:
	virtual ~Enemy();

	void Draw();
	void Update(); 

	// ‰¼‚Ì“Gíœ—pŠÖ”
	bool Destroy();

	Vector2<int> GetPos()const;
	Rect GetRect()const;
};

