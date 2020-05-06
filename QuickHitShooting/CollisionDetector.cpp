#include "CollisionDetector.h"

CollisionDetector::CollisionDetector()
{
}

CollisionDetector::~CollisionDetector()
{
}

bool CollisionDetector::IsCollision(const Vector2<int>& mousepos, const Rect& enemyrect)
{
	return ((enemyrect.Left() <= mousepos.x) && (mousepos.x <= enemyrect.Right()) && (enemyrect.Top() <= mousepos.y) && (mousepos.y <= enemyrect.Bottom()));
}
