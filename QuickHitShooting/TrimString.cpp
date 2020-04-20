#include "TrimString.h"
#include <DxLib.h>
#include "Game.h"

TrimString::TrimString()
{
	ssize = Game::Instance().GetScreenSize();
	_fontSize = 10;
}

TrimString::~TrimString()
{
}

int TrimString::GetStringCenterPosx(const std::string& name)const
{
	return (ssize.x / 2 - DxLib::GetDrawStringWidth(name.c_str(), std::strlen(name.c_str())) / 2);
}

int TrimString::GetStringRightPosx(const std::string& name, const int& rpos)const
{
	return (rpos - DxLib::GetDrawStringWidth(name.c_str(), std::strlen(name.c_str())));
}

int TrimString::GetFontSize() const
{
	return _fontSize;
}

void TrimString::ChangeFontSize(const int& fontSize)
{
	_fontSize = fontSize;
	DxLib::SetFontSize(_fontSize);
}
