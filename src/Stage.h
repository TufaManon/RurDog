#pragma once
enum class _Vaild_Input
{
	UP, DOWN, LEFT, RIGHT, OK, CANCEL, MENU,
};

struct Key
{
	bool flag;
	_Vaild_Input key;
};

class Stage
{
public:
	virtual void HandleInput(Key key) = 0;
};