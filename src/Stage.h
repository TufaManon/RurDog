#pragma once
#include <string>
#include <SDL.h>
#include <queue>
#include <array>
#define STAGE_MODULE_BEGIN namespace RurDog {
#define STAGE_MODULE_END }
using Tricolor = uint8_t;
constexpr auto TRICOLOR_MAX = 255;
constexpr auto ALPHA_MAX = 1.0F;
constexpr auto ALPHA_MIN = 0.0F;

STAGE_MODULE_BEGIN

enum class _Tetrmino_Class
{
	I, O, T, L, J, S, Z
};
#define ALL_Tetrmino_Class {_Tetrmino_Class::I, _Tetrmino_Class::O, _Tetrmino_Class::T, _Tetrmino_Class::L, _Tetrmino_Class::J, _Tetrmino_Class::S, _Tetrmino_Class::Z}
enum class _Tetrmino_State
{
	_0, R, _2, L
};
enum class _Tetrmino_Rotation_Orientation
{
	L, R
};
using Ori = _Tetrmino_Rotation_Orientation;

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

struct Color
{
	Tricolor r, g, b;
	float alpha;

	Color(int hex, float alpha = ALPHA_MAX);

	Color(std::string hex, float alpha = ALPHA_MAX);

	Color(Tricolor r = TRICOLOR_MAX,
		Tricolor g = TRICOLOR_MAX,
		Tricolor b = TRICOLOR_MAX,
		float a = ALPHA_MAX);
};

// Color Hex Date of Tetrmino
constexpr auto CYAN = "#00e09e";
constexpr auto YELLO = "#fff143";
constexpr auto PURPLE = "#8d4bbb";
constexpr auto GREEN = "#40de5a";
constexpr auto RED = "#ff461f";
constexpr auto BLUE = "#44cef6";
constexpr auto ORANGE = "#ea5506";

// Color Struct of Tetrmino
#define CYAN_COLOR Color(CYAN)
#define YELLO_COLOR Color(YELLO)
#define PURPLE_COLOR Color(PURPLE)
#define GREEN_COLOR Color(GREEN)
#define RED_COLOR Color(RED)
#define BLUE_COLOR Color(BLUE)
#define ORANGE_COLOR Color(ORANGE)

struct Coordinate
{
	int16_t x, y;
	
	// the default coordinate is (0,0)
	Coordinate(int16_t x = 0, int16_t y = 0);
};

#define Coord(x,y) Coordinate(x,y)

#define WALL_KICK_DATE(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10) {Coord(n1, n2), Coord(n3, n4), Coord(n5, n6), Coord(n7, n8), Coord(n9, n10)}

//旋转的平移测试，将旋转后的坐标减去不同形态的偏移数据之差，来确定是否可以旋转。
const std::array<Coordinate, 5> JLSTZ_O_OFFSET = WALL_KICK_DATE(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
const std::array<Coordinate, 5> JLSTZ_R_OFFSET = WALL_KICK_DATE(0, 0, +1, 0, +1, -1, 0, +2, +1, +2);
const std::array<Coordinate, 5> JLSTZ_2_OFFSET = WALL_KICK_DATE(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
const std::array<Coordinate, 5> JLSTZ_L_OFFSET = WALL_KICK_DATE(0, 0, -1, 0, -1, -1, 0, +2, -1, +2);

//旋转的平移测试，将旋转后的坐标减去不同形态的偏移数据之差，来确定是否可以旋转。
const std::array<Coordinate, 5> I_O_OFFSET = WALL_KICK_DATE(0, 0, -1, 0, +2, 0, -1, 0, +2, 0);
const std::array<Coordinate, 5> I_R_OFFSET = WALL_KICK_DATE(-1, 0, 0, 0, 0, 0, 0, +1, 0, -2);
const std::array<Coordinate, 5> I_2_OFFSET = WALL_KICK_DATE(-1, +1, +1, +1, -2, +1, +1, 0, -2, 0);
const std::array<Coordinate, 5> I_L_OFFSET = WALL_KICK_DATE(0, +1, 0, +1, 0, +1, 0, -1, 0, +2);
struct Brick
{
	//It indicates the brick is non-existent
	static const uint8_t BRICK_INVALIDE = 0;

	// It indicates that the brick exists and needs to be calculated as an obstacle
	static const uint8_t BRICK_VALIDE = 1;

	// the color is used by renderer to set draw color of brick
	Color color;
	
	//the coord is relative to the playfield wicth is controled by a player or the bounding box;
	Coordinate relative_coord;

	//indicating whether the brick is exists
	uint8_t flag;
};

struct Tetrmino
{
	using Box = SDL_Rect;
	// the bounding box different between I or O and S or Z or L or T or  J. the former has 4*4 box and the latter has 3*3 box;
	Box bounding_box;

	// the pieces witch has coordinate relative to the bounding box;
	Brick pieces[4];

	_Tetrmino_Class type;

	_Tetrmino_State state;
	
	// set all 4 brick color
	void SetColor(Color color);

	// set brick of the terimino color by index
	void SetColor(Color color, int index);

	Coordinate* GetOffSet();
};

class Random_Generator
{
	const _Tetrmino_Class TYPE_HEAD[7] = ALL_Tetrmino_Class;
public:
	_Tetrmino_Class GetNextTetrMinoType();
};

struct Harm
{

};

class Stage
{

};

class Play_Field
{
	Brick field_brick[22][10];
	Tetrmino* falling;
	Tetrmino* hold;
	std::queue<Harm> received_harm_queue;
	std::queue<Harm> emitted_harm_queue;
	std::queue<Tetrmino*> coming_tetrminoes;
public:
	void Locking();
	void Swaping();
	bool TestingRotation(Tetrmino& mino, Ori orientation);
	bool Moving(Ori orientation);
};

STAGE_MODULE_END
