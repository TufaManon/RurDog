#pragma once
#include <array>
#include "utils.hpp"
#include "coordinate.hpp"
#include "color.h"
#include "piece.hpp"
#define WALL_KICK_DATE(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10) {Coord(n1, n2), Coord(n3, n4), Coord(n5, n6), Coord(n7, n8), Coord(n9, n10)}
GAME_MODULE_BEGIN
enum class _Tetrmino_Class
{
	I, O, T, L, J, S, Z
};

enum class _Tetrmino_State
{
	_0, R, _2, L
};

inline const _Tetrmino_State operator>>(const _Tetrmino_State& state, int i)
{
	auto c = ((int)state + i);
	return c >= 0 ? (_Tetrmino_State)(c % 4) : (_Tetrmino_State)(4 + (c % 4));
}

inline const _Tetrmino_State operator<<(const _Tetrmino_State& state, int i)
{
	auto c = ((int)state - i);
	return c >= 0 ? (_Tetrmino_State)(c % 4) : (_Tetrmino_State)(4 + (c % 4));
}

enum class _Tetrmino_Rotation_Orientation
{
	L, R
};
using _Ori = _Tetrmino_Rotation_Orientation;

constexpr auto IS_I(_Tetrmino_Class type)
{
	return type == _Tetrmino_Class::I;
}
constexpr auto IS_O(_Tetrmino_Class type)
{
	return type == _Tetrmino_Class::O;
}
constexpr auto IS_T(_Tetrmino_Class type)
{
	return type == _Tetrmino_Class::T;
}
constexpr auto IS_L(_Tetrmino_Class type)
{
	return type == _Tetrmino_Class::L;
}
constexpr auto IS_J(_Tetrmino_Class type)
{
	return type == _Tetrmino_Class::J;
}
constexpr auto IS_S(_Tetrmino_Class type)
{
	return type == _Tetrmino_Class::S;
}
constexpr auto IS_Z(_Tetrmino_Class type)
{
	return type == _Tetrmino_Class::Z;
}





//旋转的平移测试，将旋转后的坐标减去不同形态的偏移数据之差，来确定是否可以旋转。
// It is the data to Testing how the tetrmino ratate by computing the difference of the different state;



inline std::array<Coordinate, 5> operator-(const std::array<Coordinate, 5>& left, const std::array<Coordinate, 5>& right)
{
	return { left.at(0) - right.at(0), left.at(1) - right.at(1), left.at(2) - right.at(2), left.at(3) - right.at(3), left.at(4) - right.at(4) };
}

struct Tetrmino
{
	// the bounding box different between I or O and S or Z or L or T or  J. the former has 4*4 box and the latter has 3*3 box;
	Coordinate bounding_box;

	// the pieces witch has coordinate relative to the bounding box;
	std::array<Piece, 4> pieces;

	_Tetrmino_Class type;

	_Tetrmino_State state;

	bool pre_locked = false;

	// set all 4 brick color
	void SetColor(const RGBA* color);

	std::array<Coordinate, 5> GetOffSet(_Tetrmino_State state) const
	{
		return GetOffsetTable(this->state) - GetOffsetTable(state);
	}
	const Coordinate GetOOffset(_Tetrmino_State state) const
	{
		return GetOOffsetTable(this->state) - GetOOffsetTable(state);
	}

	Tetrmino* Rotate(_Ori ori, int test = 0) const;

private:

	const _Tetrmino_State GetNextState(_Ori ori) const
	{
		if (ori == _Ori::R)
		{
			return state >> 1;
		}
		else return state << 1;
	}

	const void CenterRotation(_Ori ori);

	const std::array<Coordinate, 5>& GetOffsetTable(_Tetrmino_State state) const;

	const Coordinate GetOOffsetTable(_Tetrmino_State state) const;

};
using Mino = Tetrmino;
GAME_MODULE_END