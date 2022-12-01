#include "RurDog.h"

Mino_Type Bag::PopType()
{
	if (heap_tail == 7)
	{
		for (int i = 0; i < 7; i++)
		{
			int rand_index = (rand() % (7 - i)) + i;
			Mino_Type temp = type_heap[i];
			type_heap[i] = type_heap[rand_index];
			type_heap[rand_index] = temp;
			heap_tail = 0;
		}
	}
	return type_heap[heap_tail++];
}

Unit::Unit()
{
	actor = nullptr;
	for (int i = 0; i < 23; i++)
		for (int j = 0; j < 10; j++) 
		{
			world[i][j].region_coord.x = j;
			world[i][j].region_coord.y = i;
		}
}

Mino* Unit::MinoCreator()
{
	Mino_Type type = bag.PopType();
	Mino* mino = new Mino();
	mino->type = type;
	switch (type)
	{
	case Mino_Type::I:
		mino->SetBlockColor(Color(125, 245, 31, 1));
		mino->SetBlockCoord(Coord(5, 1), Coord(-2, 0), Coord(-1, 0), Coord(1, 0));
		break;
	case Mino_Type::O:
		mino->SetBlockColor(Color(248, 95, 115, 1));
		mino->SetBlockCoord(Coord(5, 1), Coord(-1, -1), Coord(-1, 0), Coord(0, -1));
		break;
	case Mino_Type::T:
		mino->SetBlockColor(Color(94, 115, 200, 1));
		mino->SetBlockCoord(Coord(5, 1), Coord(-1, 0), Coord(0, -1), Coord(1, 0));
		break;
	case Mino_Type::L:
		mino->SetBlockColor(Color(125, 31, 245, 1));
		mino->SetBlockCoord(Coord(5, 1), Coord(-1, 0), Coord(1, 0), Coord(1, -1));
		break;
	case Mino_Type::J:
		mino->SetBlockColor(Color(125, 245, 31, 1));
		mino->SetBlockCoord(Coord(5, 1), Coord(-1, -1), Coord(-1, 0), Coord(1, 0));
		break;
	case Mino_Type::S:
		mino->SetBlockColor(Color(125, 2, 31, 1));
		mino->SetBlockCoord(Coord(5, 1), Coord(-1, 0), Coord(0, -1), Coord(1, -1));
		break;
	case Mino_Type::Z:
		mino->SetBlockColor(Color(1, 222, 31, 1));
		mino->SetBlockCoord(Coord(5, 1), Coord(-1, -1), Coord(0, -1), Coord(1, 0));
		break;
	default:
		break;
	}
	return mino;
}

bool Unit::HasActor()
{
	return actor != nullptr;
}

void Unit::ActorMoveLeft()
{
}

void Unit::ActorMoveRight()
{
}

void Unit::ActorSinistralRotation()
{
}

void Unit::ActorDextralRotation()
{
}

void Unit::ActorFall()
{
	auto begin = signal.begin();
	for (; begin != signal.end(); begin++)
	{
		if (*begin == FALL_COMPELET)
			return;
	}
	for (int i = 0; i < 4; i++)
	{
		if (!world[actor->blocks[i].region_coord.y + 1][actor->blocks[i].region_coord.x].empty_block)
		{
			signal.push_back(FALL_COMPELET);
			return;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (actor->blocks[i].region_coord.y < 22)
		{
			actor->blocks[i].region_coord.y++;
		}
		else
		{
			signal.push_back(FALL_COMPELET);
			return;
		}
	}
}

void Unit::LockActor()
{
	for (int i = 0; i < 4; i++)
	{
		world[actor->blocks[i].region_coord.y][actor->blocks[i].region_coord.x] = actor->blocks[i];
	}
	delete actor;
	actor = nullptr;
}

void Unit::CreatActor()
{
	actor = MinoCreator();
}

void Mino::SetBlockColor(Color color)
{
	for (int i = 0; i < 4; i++)
	{
		blocks[i].color = color;
	}
}

void Mino::SetBlockCoord(Coord center, Coord c1, Coord c2, Coord c3)
{
	blocks[0].region_coord = center;
	blocks[1].region_coord = Coord(c1.x + center.x, c1.y + center.y);
	blocks[2].region_coord = Coord(c2.x + center.x, c2.y + center.y);
	blocks[3].region_coord = Coord(c3.x + center.x, c3.y + center.y);
}
