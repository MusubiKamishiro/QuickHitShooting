#pragma once
#include <DxLib.h>

class Input
{
public:
	Input();
	~Input();

	const bool IsPressed(const int& p) const;
	const bool IsTrigger(const int& p) const;

	const bool IsMousePressed(const int& p);
	const bool IsMouseTrigger(const int& p);

	void Update();

private:
	char _keyData[256];
	char _keyDataOld[256];

	int _nowInput;
	int _oldInput;
};

