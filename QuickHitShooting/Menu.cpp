#include "Menu.h"
#include <DxLib.h>
#include "Peripheral.h"
#include "CollisionDetector.h"

Menu::Menu()
{
	_cd.reset(new CollisionDetector());
}


Menu::~Menu()
{
}

void Menu::AddMenuList(const std::string& name, const Vector2<int>& pos, const Size& size, const int& img)
{
	MenuData md = MenuData(Rect(pos, size), img);
	_menuTable.emplace(name, md);

	_menuTable[name.c_str()];
}

bool Menu::CheckCrick(const std::string& name, const Peripheral& p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT) && _cd->IsCollision(p.GetMousePos(), _menuTable[name.c_str()].rect))
	{
		return true;
	}
	return false;
}

void Menu::Update(const Peripheral & p)
{
	
}

void Menu::Draw()
{
	for (auto& m : _menuTable)
	{
		MenuData md = m.second;
		DxLib::DrawGraph(md.rect.Left(), md.rect.Top(), md.img, true);
	}
}
