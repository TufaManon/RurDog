#include "Stage.h"
STAGE_MODULE_BEGIN
_Tetrmino_Class Random_Generator::GetNextTetrMinoType()
{
	if (TouchTheTop())
	{
		for (int i = 0; i < 7; i++)
		{
			int rand_index = (rand() % (7 - i)) + i;
			SWAP(types[i], types[rand_index]);
			InitTopPoint();
		}
	}
	top++;
	return types[top - 1];
}

void Tetrmino::SetColor(RGBA color)
{
	for (int i = 0; i < pieces.size(); i++)
	{
		pieces.at(i).color = color;
	}
}

void Tetrmino::SetColor(HSLA color)
{
	SetColor(color.ToRGBA());
}

void Tetrmino::SetColor(RGBA color, int index)
{
	pieces.at(index).color = color;
}

void Tetrmino::SetColor(HSLA color, int index)
{
	SetColor(color.ToRGBA(), index);
}

Tetrmino& Tetrmino::operator=(Tetrmino& right)
{
	this->bounding_box = right.bounding_box;
	this->state = right.state;
	this->type = right.type;
	for (int i = 0; i < 4; i++)
		this->pieces[i] = right.pieces[i];
	return *this;
}

const Tetrmino Tetrmino::Rotate(Ori ori, int test) const
{
	auto s = GetNextState(ori);
	auto mino = *this;
	if (type == _Tetrmino_Class::O)
	{
		auto i = GetOOffset(s);
		mino.bounding_box = bounding_box - i;
		return mino;
	}
	else
	{
		auto is = GetOffSet(s);
		mino.bounding_box = bounding_box - is.at(test);
		return mino;
	}
}

const void Tetrmino::CenterRotation(Ori ori)
{
	int cos = 1, sin, msin;
	if (ori == Ori::L)
	{
		sin = -1;
		msin = 1;
	}
	else
	{
		sin = 1;
		msin = -1;
	}
	for (auto& p : pieces)
	{
		p.relative_coord.x = cos * p.relative_coord.x + msin * p.relative_coord.y;
		p.relative_coord.y = sin * p.relative_coord.x + cos * p.relative_coord.y;
	}
}

void Signaler::Emit(int signal)
{
	if(map.contains(signal))
		for (auto fn : map[signal])
		{
			fn.fn();
		}
}

Signaler::Signaler(std::initializer_list<int> signals)
{
	for (auto sign : signals)
	{
		const std::vector<Fn> stack;
		map[sign] = stack;
	}
}

int Signaler::ConnectSignal(int signal, std::function<void(void)> fn)
{
	if (map.contains(signal))
	{
		Fn s;
		s.flag = this->flag;
		this->flag++;
		s.fn = fn;
		map[signal].push_back(s);
		return this->flag - 1;
	}
}

void Signaler::DesconnectSignal(int signal, int flag)
{
	if (map.contains(signal))
	{
		for (auto it = map[signal].begin(); it != map[signal].end();)
		{
			if (it->flag == flag) it = map[signal].erase(it);
			else it++;
		}
	}
}

std::array<Coordinate, 4> Play_Field::Get_Field_Coord(const Tetrmino& mino) const
{
	std::array<Coordinate, 4> result;
	auto& box = mino.bounding_box;
	auto& array = mino.pieces;
	for (int i = 0; i < array.size(); i++)
	{
		result[i] = Coord(box.x + array.at(i).relative_coord.x, box.y + array.at(i).relative_coord.y);
	}
	return result;
}

bool Play_Field::IsNotInOBSTACLE(const Tetrmino& mino) const
{
	auto i = Get_Field_Coord(mino);
	bool res = true;
	for (const auto& c : i)
	{
		res &= (c.y < 22 && c.x < 10) && (field_brick[c.y][c.x].flag == Brick::BRICK_INVALIDE);
	}
	return res;
}

constexpr int MINO_COORD[][8] = { 
	{-1, 0, 0, 0, 1, 0, 2, 0},
	{-1, -1, -1, 0, 0, 0, 1, 0},
	{-1, 0, 0, 0, 1, 0, 1, -1},
	{0, -1, 0, 0, 1, -1, 1, 0},
	{-1, 0, 0, 0, 0, -1, 1, -1},
	{-1, 0, 0, -1, 0, 0, 1, 0},
	{-1, -1, 0, -1, 0, 0, 1, 0} };

Tetrmino* Play_Field::CreateTetrmino()
{
	auto type = generator.GetNextTetrMinoType();
	Tetrmino* mino = new Tetrmino();
		mino->bounding_box = Coord(4,1);
		mino->state = _Tetrmino_State::_0;
	switch (type)
	{
	case RurDog::_Tetrmino_Class::I:
		for (int i = 0; i < 4; i++)
		{
			mino->pieces.at(i).relative_coord = Coord(MINO_COORD[0][i * 2],MINO_COORD[0][i * 2 + 1]);
			mino->SetColor(CYAN_COLOR);
		}
		break;
	case RurDog::_Tetrmino_Class::O:
		for (int i = 0; i < 4; i++)
		{
			mino->pieces.at(i).relative_coord = Coord(MINO_COORD[3][i * 2],MINO_COORD[3][i * 2 + 1]);
			mino->SetColor(BLUE_COLOR);
		}
		break;
	case RurDog::_Tetrmino_Class::T:
		for (int i = 0; i < 4; i++)
		{
			mino->pieces.at(i).relative_coord = Coord(MINO_COORD[5][i * 2],MINO_COORD[5][i * 2 + 1]);
			mino->SetColor(ORANGE_COLOR);
		}
		break;
	case RurDog::_Tetrmino_Class::L:
		for (int i = 0; i < 4; i++)
		{
			mino->pieces.at(i).relative_coord = Coord(MINO_COORD[2][i * 2],MINO_COORD[2][i * 2 + 1]);
			mino->SetColor(YELLO_COLOR);
		}
		break;
	case RurDog::_Tetrmino_Class::J:
		for (int i = 0; i < 4; i++)
		{
			mino->pieces.at(i).relative_coord = Coord(MINO_COORD[1][i * 2],MINO_COORD[1][i * 2 + 1]);
			mino->SetColor(GREEN_COLOR);
		}
		break;
	case RurDog::_Tetrmino_Class::S:
		for (int i = 0; i < 4; i++)
		{
			mino->pieces.at(i).relative_coord = Coord(MINO_COORD[4][i * 2],MINO_COORD[4][i * 2 + 1]);
			mino->SetColor(RED_COLOR);
		}
		break;
	case RurDog::_Tetrmino_Class::Z:
		for (int i = 0; i < 4; i++)
		{
			mino->pieces.at(i).relative_coord = Coord(MINO_COORD[6][i * 2],MINO_COORD[6][i * 2 + 1]);
			mino->SetColor(BLUE_COLOR);
		}
		break;
	default:
		break;
	}
	return mino;
}

std::array<Brick, 4> Play_Field::GetFalling() const
{
	std::array<Brick, 4> res = falling->pieces;
	for (auto it = res.begin(); it != res.end(); it++)
	{
		it->relative_coord += falling->bounding_box;
	}
	return res;
}

void Play_Field::Locking()
{
	int x = falling->bounding_box.x;
	int y = falling->bounding_box.y;
	for (auto& p : falling->pieces)
	{
		p.relative_coord.x += x;
		p.relative_coord.y += y;
		p.flag = p.BRICK_VALIDE;
		field_brick[p.relative_coord.y][p.relative_coord.x] = p;
	}
	delete falling;
	falling = hold;
	hold = CreateTetrmino();
	swap = false;
}
void Play_Field::Swaping()
{
	if (swap == false)
	{
		SWAP(falling, hold)
		swap = true;
	}
}

bool Play_Field::TestingRotation(Tetrmino& mino, Ori orientation)
{
	for (int i = 0; i < 4; i++)
	{
		auto smino = mino.Rotate(orientation, i);
		if (IsNotInOBSTACLE(smino))
		{
			mino = smino;
			return true;
		}
		if (mino.type == _Tetrmino_Class::O) break;
	}
	return false;
}

inline bool Play_Field::Rotation(Ori orientation)
{
	return  TestingRotation(*falling, orientation);
}

inline bool Play_Field::Moving(Ori orientation)
{
	return TestMoving(*falling, orientation);
}

bool Play_Field::TestMoving(Tetrmino& mino, Ori orientation)
{
	Tetrmino tmino = mino;
	if (orientation == Ori::L)
		tmino.bounding_box << 1;
	else
		tmino.bounding_box >> 1;
	if (IsNotInOBSTACLE(tmino))
	{
		mino = tmino;
		return true;
	}
	return false;
}

bool Play_Field::Down()
{
	int res = TestDown(*falling);
	if (!res) Emit(this->SIGNAL_TOUCH_OBSTACLE);
	return res;
}

bool Play_Field::TestDown(Tetrmino& mino)
{
	Tetrmino tmino = mino;
	tmino.bounding_box ^ 1;
	if (IsNotInOBSTACLE(tmino))
	{
		mino = tmino;
		return true;
	}
	return false;
}

STAGE_MODULE_END

