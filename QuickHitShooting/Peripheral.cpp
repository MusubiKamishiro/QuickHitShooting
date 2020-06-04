#include <DxLib.h>
#include "Peripheral.h"
#include "Game.h"
#include "Loader/FileSystem.h"
#include "Loader/ImageLoader.h"

Peripheral::Peripheral()
{
	oldMouseState = 0;
	mouseState	  = 0;
	mousePos	  = Vector2<int>(0, 0);
	DxLib::SetMouseDispFlag(false);

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/target.png", data);
	pointImg = data.GetHandle();
}


Peripheral::~Peripheral()
{
}

void Peripheral::Update()
{
	// マウスの更新
	oldMouseState = mouseState;
	mouseState = DxLib::GetMouseInput();

	DxLib::GetMousePoint(&mousePos.x, &mousePos.y);
}

bool Peripheral::IsPressing(const int& cmd) const
{
	return ((oldMouseState & cmd) && (mouseState & cmd));
}

bool Peripheral::IsTrigger(const int& cmd) const
{
	return (!(oldMouseState & cmd) && (mouseState & cmd));
}

Vector2<int> Peripheral::GetMousePos() const
{
	return mousePos;
}

void Peripheral::Draw()
{
	DxLib::DrawExtendGraph(mousePos.x - 35, mousePos.y - 35, mousePos.x + 35, mousePos.y + 35,
						   pointImg, true);
}
