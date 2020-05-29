#pragma once
#include <memory>
#include <string>
#include "Geometry.h"

class Menu;
class Peripheral;

class Keyboard
{
private:
	std::shared_ptr<Menu> _menu;
	std::string _name;

	unsigned int _nameCount;

public:
	Keyboard();
	~Keyboard();

	bool Update(const Peripheral& p);

	void Draw();

	std::string GetName()const;
};

