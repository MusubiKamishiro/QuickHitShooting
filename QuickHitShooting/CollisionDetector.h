#pragma once
#include "Geometry.h"


class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();

	// �����蔻��
	bool IsCollision(const Vector2<int>& mousepos, const Rect& enemyrect);
};
