#pragma once
#include <stdint.h>
#include <cstdlib>
#include <vector>
#define GAME_NAME "RurDog"
enum class Mino_Type
{
	I, O, T, L, J, S, Z
};
struct Color
{
	uint8_t r, g, b, a;
	Color():r(0),g(0),b(0),a(1){}
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 1)
	{
		this->r = r;
		this->g = g;
		this->b = b;;
		this->a = a;
	}
};
struct Coord
{
	int16_t x, y;
	Coord():x(0),y(0) {};
	Coord(int16_t x, int16_t y)
	{
		this->x = x;
		this->y = y;
	};
};
struct Region
{
	Coord coord;
	uint16_t tile_size;
	uint8_t w, h;
};
struct Block
{
	Coord region_coord;
	Color color;
	bool empty_block = true;
};
struct Mino
{
	Block blocks[4];
	Mino_Type type;
	Mino():type(Mino_Type::I)
	{
		for (int i = 0; i < 4; i++)
			blocks[i].empty_block = false;
	}
	void SetBlockColor(Color color);
	void SetBlockCoord(Coord center, Coord c1, Coord c2, Coord c3);
};

struct Bag
{
	Mino_Type type_heap[7] =
		{ Mino_Type::I,Mino_Type::J,Mino_Type::L,Mino_Type::O,Mino_Type::S,Mino_Type::T,Mino_Type::Z };
	int heap_tail = 0;
	Mino_Type PopType();
};

class Unit
{
public:
	const int FALL_COMPELET = 0;
	Unit();
	Region region;
	Block world[23][10];
	std::vector<int> signal;
	Bag bag;
	Mino* actor;

	Mino* MinoCreator();

	bool HasActor();

	void ActorMoveLeft();

	void ActorMoveRight();
	//方块左旋
	void ActorSinistralRotation();
	//方块右旋
	void ActorDextralRotation();

	void ActorFall();

	void LockActor();

	void CreatActor();
	
};
