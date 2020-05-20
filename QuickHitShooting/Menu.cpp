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

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/white.png", data);
	img = data.GetHandle();

	size = 20;
}


Menu::~Menu()
{
}

void Menu::AddMenuList(const std::string& name, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const int& img)
{
	MenuData md = MenuData(Rect((rbPos.x + ltPos.x)/2, (rbPos.y + ltPos.y)/2, std::abs(rbPos.x - ltPos.x), std::abs(rbPos.y - ltPos.y)), img);
	_menuTable.emplace(name, md);
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
			DxLib::DrawExtendGraph(md.rect.Left() - size, md.rect.Top() - size, md.rect.Right() + size, md.rect.Bottom() + size, img, true);
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
		}

		//DxLib::DrawGraph(md.rect.Left(), md.rect.Top(), md.img, true);
		DxLib::DrawExtendGraph(md.rect.Left(), md.rect.Top(), md.rect.Right(), md.rect.Bottom(), md.img, true);

		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
}
