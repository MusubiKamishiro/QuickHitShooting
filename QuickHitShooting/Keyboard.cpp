#include "Keyboard.h"
#include "Menu.h"

#include "Game.h"
#include "Loader/FileSystem.h"
#include "Loader/ImageLoader.h"

Keyboard::Keyboard()
{
	_menu.reset(new Menu());

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/keyboard.png", data);
	int img = data.GetHandle();

	int count = 0;

	for (char i = 'A'; i <= 'Z'; ++i)
	{
		Vector2<int> keypos = Vector2<int>(50 * (count % 10), 50 * (count / 10));
		_menu->AddMenuList({ i }, keypos, keypos + Vector2<int>(50, 50), Rect(keypos + Vector2<int>(25, 25), Size(50, 50)), img);
		
		++count;
	}
}

Keyboard::~Keyboard()
{
}

void Keyboard::Update(const Peripheral& p)
{
	_menu->Update(p);
}

void Keyboard::Draw(const Vector2<int>& pos)
{
	_menu->Draw();
}
