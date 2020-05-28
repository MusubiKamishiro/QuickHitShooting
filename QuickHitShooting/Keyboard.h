#pragma once
#include <memory>
#include "Geometry.h"

class Menu;
class Peripheral;

class Keyboard
{
private:
	std::shared_ptr<Menu> _menu;

public:
	Keyboard();
	~Keyboard();

	void Update(const Peripheral& p);

	void Draw(const Vector2<int>& pos);
};

