#include "Keyboard.h"
#include <DxLib.h>
#include "Menu.h"

#include "Game.h"
#include "Loader/FileSystem.h"
#include "Loader/ImageLoader.h"

Keyboard::Keyboard()
{
	_menu.reset(new Menu());
	_name = "---";

	_nameCount = 0;
	
	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/keyboard.png", data);
	int img = data.GetHandle();

	int count = 0;
	Vector2<int> scrSize = Game::Instance().GetScreenSize();
	Vector2<int> keypos;

	for (char i = 'A'; i <= 'Z'; ++i)
	{
		keypos = Vector2<int>(100 * (count % 10), 100 * (count / 10)) + Vector2<int>(140, scrSize.y/2);
		_menu->AddMenuList({ i }, keypos, keypos + Vector2<int>(100, 100), Rect(Vector2<int>(50 * (count % 10), 50 * (count / 10)) + Vector2<int>(25, 25), Size(50, 50)), img);
		
		++count;
	}

	keypos = Vector2<int>(100 * (count % 10), 100 * (count / 10)) + Vector2<int>(140, scrSize.y / 2);
	_menu->AddMenuList("del", keypos, keypos + Vector2<int>(200, 100), Rect(Vector2<int>(50 * (count % 10), 50 * (count / 10)) + Vector2<int>(50, 25), Size(100, 50)), img);
	count += 2;

	keypos = Vector2<int>(100 * (count % 10), 100 * (count / 10)) + Vector2<int>(140, scrSize.y / 2);
	_menu->AddMenuList("enter", keypos, keypos + Vector2<int>(200, 100), Rect(Vector2<int>(50 * (count % 10), 50 * (count / 10)) + Vector2<int>(50, 25), Size(100, 50)), img);
}

Keyboard::~Keyboard()
{
}

bool Keyboard::Update(const Peripheral& p)
{
	_menu->Update(p);

	for (char i = 'A'; i <= 'Z'; ++i)
	{
		if (_menu->CheckClick({ i }, p) && (_nameCount < 3))
		{
			_name[_nameCount] = { i };
			++_nameCount;
		}
	}

	if (_menu->CheckClick("del", p) && (_nameCount > 0))
	{
		--_nameCount;
		_name[_nameCount] = '-';
	}
	else if (_menu->CheckClick("enter", p))
	{
		return true;
	}

	return false;
}

void Keyboard::Draw()
{
	DxLib::DrawString(100, 100, _name.c_str(), 0xff0000);

	_menu->Draw();
}

std::string Keyboard::GetName() const
{
	return _name;
}
