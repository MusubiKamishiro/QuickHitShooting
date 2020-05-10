#pragma once
#include "Geometry.h"


class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();

	// “–‚½‚è”»’è
	bool IsCollision(const Vector2<int>& mousepos, const Rect& enemyrect);
};
