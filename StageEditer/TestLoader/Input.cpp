#include "Input.h"

Input::Input()
{
	for (int i = 0; i < sizeof(_keyData) / sizeof(_keyData[0]); ++i)
	{
		_keyData[i]	   = 0;
		_keyDataOld[i] = 0;
	}
}

Input::~Input()
{
}

const bool Input::IsPressed(const int& p) const
{
	return _keyData[p];
}

const bool Input::IsTrigger(const int& p) const
{
	return (_keyData[p] && !_keyDataOld[p]);
}

const bool Input::IsMousePressed(const int& p)
{
	return (_nowInput && p);
}

const bool Input::IsMouseTrigger(const int& p)
{
	return (_nowInput && !_oldInput);
}

void Input::Update()
{
	memcpy(_keyDataOld, _keyData, sizeof(_keyDataOld));
	
	_oldInput = _nowInput;
	_nowInput = GetMouseInput();

	GetHitKeyStateAll(_keyData);

}