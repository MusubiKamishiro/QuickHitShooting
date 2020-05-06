#include "Peripheral.h"
#include <DxLib.h>


Peripheral::Peripheral()
{
	oldMouseState = 0;
	mouseState = 0;
	mousePos = Vector2<int>(0, 0);
	DxLib::SetMouseDispFlag(false);
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

void Peripheral::DebugDraw()
{
	//DxLib::DrawBox(0, 0, 400, 200, 0xffffff, true);
	DxLib::SetFontSize(20);
	DxLib::DrawFormatString(0, 0, 0x000000, "マウスの座標(%d, %d)", mousePos.x, mousePos.y);
	DxLib::DrawBox(mousePos.x - 5, mousePos.y - 5, mousePos.x + 5, mousePos.y + 5, 0x0000ff, true);
}
