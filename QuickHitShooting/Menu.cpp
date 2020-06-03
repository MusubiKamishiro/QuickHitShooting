#include "Menu.h"
#include <DxLib.h>
#include <cmath>
#include "Peripheral.h"
#include "CollisionDetector.h"

#include "Game.h"
#include "Loader/FileSystem.h"
#include "Loader/ImageLoader.h"

Menu::Menu()
{
	_cd.reset(new CollisionDetector());
}


Menu::~Menu()
{
}

void Menu::AddMenuList(const std::string& name, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const int& img)
{
	Vector2<int> graphSize;
	DxLib::GetGraphSize(img, &graphSize.x, &graphSize.y);
	MenuData md = MenuData(img, Rect((rbPos.x + ltPos.x)/2, (rbPos.y + ltPos.y)/2, std::abs(rbPos.x - ltPos.x), std::abs(rbPos.y - ltPos.y)), Rect(graphSize / 2, Size(graphSize)));
	_menuTable.emplace(name, md);
}

void Menu::AddMenuList(const std::string& name, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const Rect& imgRect, const int& img)
{
	MenuData md = MenuData(img, Rect((rbPos.x + ltPos.x) / 2, (rbPos.y + ltPos.y) / 2, std::abs(rbPos.x - ltPos.x), std::abs(rbPos.y - ltPos.y)), imgRect);
	_menuTable.emplace(name, md);
}

bool Menu::CheckClick(const std::string& name, const Peripheral& p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT) && _cd->IsCollision(p.GetMousePos(), _menuTable[name.c_str()].rect))
	{
		return true;
	}
	return false;
}

void Menu::Update(const Peripheral & p)
{
	auto pos = p.GetMousePos();

	for (auto& m : _menuTable)
	{
		if (_cd->IsCollision(pos, m.second.rect))
		{
			m.second.flag = true;
		}
		else
		{
			m.second.flag = false;
		}
	}
}

void Menu::Draw()
{
	for (auto& m : _menuTable)
	{
		MenuData md = m.second;

		if (md.flag)
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
		}

		DxLib::DrawRectExtendGraph(md.rect.Left(), md.rect.Top(), md.rect.Right(), md.rect.Bottom(),
			md.useRect.Left(), md.useRect.Top(), md.useRect.Width(), md.useRect.Height(), md.img, true);

		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
}
