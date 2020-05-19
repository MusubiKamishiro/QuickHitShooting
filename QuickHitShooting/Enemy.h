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

	virtual void Draw() = 0;
	void Update(); 

	// ’e‚ª“–‚½‚Á‚½‚Ìˆ—
	void HitShot();

	// ‰¼‚Ì“Gíœ—pŠÖ”
	bool Destroy();

	virtual int GetPoint() const = 0;
	Vector2<int> GetPos() const;
	Rect GetRect() const;
};

