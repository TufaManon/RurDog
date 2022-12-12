#pragma once
#include "utils.hpp"
GAME_MODULE_BEGIN
struct Coordinate
{
	int16_t x, y;

	// the default coordinate is (0,0)
	Coordinate(int16_t x = 0, int16_t y = 0)
	{
		this->x = x;
		this->y = y;
	}
	const Coordinate operator+(const Coordinate& right) const
	{
		return Coordinate(x + right.x, y + right.y);
	}
	const Coordinate& operator+=(const Coordinate& right)
	{
		this->x += right.x;
		this->y += right.y;
		return *this;
	}

	const Coordinate& operator-=(const Coordinate& right)
	{
		this->x -= right.x;
		this->y -= right.y;
		return *this;
	}

	Coordinate& operator=(const Coordinate& right)
	{
		this->x = right.x;
		this->y = right.y;
		return *this;
	}

	const Coordinate operator-(const Coordinate& right) const
	{
		return Coordinate(x - right.x, y - right.y);
	}
	bool operator==(const Coordinate& right) const
	{
		return x == right.x && y == right.y;
	}
	const Coordinate& operator<<(int off)
	{
		this->x -= off;
		return *this;
	}
	const Coordinate& operator>>(int off)
	{
		this->x += off;
		return *this;
	}

	const Coordinate& operator^(int off)
	{
		this->y += off;
		return *this;
	}
};

#define Coord(x,y) Coordinate(x,y)
GAME_MODULE_END