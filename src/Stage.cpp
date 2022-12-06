#include "Stage.h"
GAME_MODULE_BEGIN
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

void Tetrmino::SetColor(const RGBA& color)
{
	for (int i = 0; i < pieces.size(); i++)
	{
		pieces.at(i).color = color;
	}
}
Tetrmino& Tetrmino::operator=(const Tetrmino& right)
{
	this->bounding_box = right.bounding_box;
	this->state = right.state;
	this->type = right.type;
	for (int i = 0; i < 4; i++)
		this->pieces[i] = right.pieces[i];
	return *this;
}

Tetrmino::Tetrmino(const Tetrmino& right)
{
	*this = right;
}

const Tetrmino Tetrmino::Rotate(_Ori ori, int test) const
{
	auto s = GetNextState(ori);
	Tetrmino mino = *this;
	mino.state = s;
	mino.CenterRotation(ori);
	if (type == _Tetrmino_Class::O)
	{
		auto i = GetOOffset(s);
		mino.bounding_box = bounding_box + i;
		return mino;
	}
	else
	{
		auto is = GetOffSet(s);
		mino.bounding_box = bounding_box + is.at(test);
		return mino;
	}
}

const void Tetrmino::CenterRotation(_Ori ori)
{
	int cos = 0, sin, msin;
	if (ori == _Ori::L)
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
		auto x = p.relative_coord.x;
		p.relative_coord.x = cos * p.relative_coord.x + msin * p.relative_coord.y;
		p.relative_coord.y = sin * x + cos * p.relative_coord.y;
	}
}

void Signaler::Emit(int signal)
{
	if (map.contains(signal))
		for (auto& flagfn : map[signal])
		{
			flagfn.fn();
		}
}

Signaler::Signaler(std::initializer_list<int> signals)
{
	for (auto sign : signals)
	{
		const std::vector<FlagFn> stack;
		map[sign] = stack;
	}
}

int Signaler::ConnectSignal(int signal, std::function<void(void)> fn)
{
	if (map.contains(signal))
	{
		FlagFn s;
		s.flag = this->flag;
		this->flag++;
		s.fn = fn;
		map[signal].push_back(s);
		return this->flag - 1;
	}

	return -1;
}

void Signaler::DisconnectSignal(int signal, int flag)
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

bool Play_Field::IsNotInObstacle(const Tetrmino& mino) const
{
	auto i = Get_Field_Coord(mino);
	bool res = true;
	for (const auto& c : i)
	{
		res &= (c.y < 22 && c.x < 10 && c.y >= 0 && c.x >= 0) && (field_backs[c.y][c.x].flag == Piece::BRICK_INVALIDE);
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
	mino->bounding_box = Coord(4, 1);
	mino->state = _Tetrmino_State::_0;
	mino->type = type;
	switch (type)
	{
	case RurDog::_Tetrmino_Class::I:
		for (int i = 0; i < 4; i++)
		{
			mino->pieces.at(i).relative_coord = Coord(MINO_COORD[0][i * 2], MINO_COORD[0][i * 2 + 1]);
			mino->SetColor(CYAN_COLOR);
		}
		break;
	case RurDog::_Tetrmino_Class::O:
		for (int i = 0; i < 4; i++)
		{
			mino->pieces.at(i).relative_coord = Coord(MINO_COORD[3][i * 2], MINO_COORD[3][i * 2 + 1]);
			mino->SetColor(YELLO_COLOR);
		}
		break;
	case RurDog::_Tetrmino_Class::T:
		for (int i = 0; i < 4; i++)
		{
			mino->pieces.at(i).relative_coord = Coord(MINO_COORD[5][i * 2], MINO_COORD[5][i * 2 + 1]);
			mino->SetColor(PURPLE_COLOR);
		}
		break;
	case RurDog::_Tetrmino_Class::L:
		for (int i = 0; i < 4; i++)
		{
			mino->pieces.at(i).relative_coord = Coord(MINO_COORD[2][i * 2], MINO_COORD[2][i * 2 + 1]);
			mino->SetColor(ORANGE_COLOR);
		}
		break;
	case RurDog::_Tetrmino_Class::J:
		for (int i = 0; i < 4; i++)
		{
			mino->pieces.at(i).relative_coord = Coord(MINO_COORD[1][i * 2], MINO_COORD[1][i * 2 + 1]);
			mino->SetColor(BLUE_COLOR);
		}
		break;
	case RurDog::_Tetrmino_Class::S:
		for (int i = 0; i < 4; i++)
		{
			mino->pieces.at(i).relative_coord = Coord(MINO_COORD[4][i * 2], MINO_COORD[4][i * 2 + 1]);
			mino->SetColor(GREEN_COLOR);
		}
		break;
	case RurDog::_Tetrmino_Class::Z:
		for (int i = 0; i < 4; i++)
		{
			mino->pieces.at(i).relative_coord = Coord(MINO_COORD[6][i * 2], MINO_COORD[6][i * 2 + 1]);
			mino->SetColor(RED_COLOR);
		}
		break;
	default:
		break;
	}
	return mino;
}

bool Play_Field::FallingPiecesCoordsAre(std::function<bool(const Coordinate&)> fn)
{
	auto coord = Get_Field_Coord(*falling);
	for (auto it = coord.begin(); it != coord.end(); it++)
	{
		if (!fn(*it)) return false;
	}
	return true;
}

void Play_Field::EachBackInner(std::function<void(Piece&)> fn)
{
	for (int i = 0; i < 22; i++)
		for (int j = 0; j < 10; j++)
			fn(field_backs[i][j]);
}

void Play_Field::EachBackInner(std::function<void(Piece&, int x, int y)> fn)
{
	for (int i = 0; i < 22; i++)
		for (int j = 0; j < 10; j++)
			fn(field_backs[i][j], j, i);
}

void Play_Field::EachBack(std::function<void(const Piece&)> fn) const
{
	for (int i = 0; i < 22; i++)
		for (int j = 0; j < 10; j++)
			fn(field_backs[i][j]);
}

void Play_Field::EachBack(std::function<void(const Piece&, int x, int y)> fn) const
{
	for (int i = 0; i < 22; i++)
		for (int j = 0; j < 10; j++)
			fn(field_backs[i][j], j, i);
}

std::array<Piece, 4> Play_Field::GetFallingMinoAbsCoord() const
{
	std::array<Piece, 4> res = falling->pieces;
	for (auto it = res.begin(); it != res.end(); it++)
	{
		it->relative_coord += falling->bounding_box;
	}
	return res;
}

Play_Field::Play_Field() :Signaler({ SIGNAL_LOCKED, SIGNAL_TOUCH_OBSTACLE, SIGNAL_GAME_OVER})
{
	falling = CreateTetrmino();
	hold = nullptr;
	EachBackInner([](Piece& p, int x, int y)->void {
		p.relative_coord = Coord(x, y);
		p.color = BACKGROUND_COLOR;
		});
	for (int i = 0; i < 4; i++)
		next_queue.push(CreateTetrmino());
}

void Play_Field::Lock()
{
	if (game_over) return;
	if (!FallingPiecesCoordsAre([](const Coordinate& coord)->bool {return coord.y >= 2; }))
	{
		game_over = true;
		Emit(SIGNAL_GAME_OVER);
		return;
	}
	int x = falling->bounding_box.x;
	int y = falling->bounding_box.y;
	for (auto& piece : falling->pieces)
	{
		piece.relative_coord += falling->bounding_box;
		piece.flag = piece.BRICK_VALIDE;
		field_backs[piece.relative_coord.y][piece.relative_coord.x] = piece;
	}
	delete falling;
	Eli();
	if (hold != nullptr)
	{
		falling = hold;
	}
	else
	{
		falling = next_queue.front();
		next_queue.pop();
		next_queue.push(CreateTetrmino());
	}
	if (!IsNotInObstacle(*falling))
	{
		game_over = true;
		Emit(SIGNAL_GAME_OVER);
	}
}

bool Play_Field::CheckColEli(int col)
{
	bool res = true;
	for (int x = 0; x < 10; x++)
	{
		res &= field_backs[col][x].flag == Piece::BRICK_VALIDE;
	}
	return res;
}

bool Play_Field::Eli()
{
	std::vector<int> cols;
	for (int y = 21; y >= 2; y--)
	{
		if (CheckColEli(y))
		{
			cols.push_back(y);
		}
	}

	int i = 0;
	while (i < cols.size())
	{
		int end = -1;
		if (i < cols.size() - 1)
			end = cols[i + 1] - 1;
		for (int col = cols[i] + i; col - 1 - i > end; col--)
		{
			for (int x = 0; x < 10; x++)
			{
				field_backs[col][x] = field_backs[col - 1 - i][x];
				field_backs[col][x].relative_coord = Coord(x, col);
			}
		}
		i++;
	}

	return cols.size() > 0;
}

bool Play_Field::Swap()
{
	if (game_over) return false;
	if (hold == nullptr)
	{
		hold = falling;
		falling = next_queue.front();
		next_queue.pop();
		return true;
	}
	return false;
}

bool Play_Field::TestingRotate(Tetrmino& mino, _Ori orientation) const
{
	for (int i = 0; i < 4; i++)
	{
		Tetrmino smino = mino.Rotate(orientation, i);
		if (IsNotInObstacle(smino))
		{
			mino = smino;
			return true;
		}
		if (mino.type == _Tetrmino_Class::O) break;
	}
	return false;
}

bool Play_Field::Rotate(_Ori orientation)
{
	if (game_over) return false;
	if (falling->type == _Tetrmino_Class::O) return false;
	return  TestingRotate(*falling, orientation);
}

bool Play_Field::Move(_Ori orientation)
{
	if (game_over) return false;
	return TestingMove(*falling, orientation);
}

bool Play_Field::TestingMove(Tetrmino& mino, _Ori orientation) const
{
	Tetrmino tmino = mino;
	if (orientation == _Ori::L)
		tmino.bounding_box << 1;
	else
		tmino.bounding_box >> 1;
	if (IsNotInObstacle(tmino))
	{
		mino = tmino;
		return true;
	}
	return false;
}

bool Play_Field::Drop()
{
	if (game_over) return false;
	int res = TestingDrop(*falling);
	if (!res) Emit(this->SIGNAL_TOUCH_OBSTACLE);
	return res;
}

bool Play_Field::TestingDrop(Tetrmino& mino) const
{
	Tetrmino tmino = mino;
	tmino.bounding_box ^ 1;
	if (IsNotInObstacle(tmino))
	{
		mino = tmino;
		return true;
	}
	return false;
}
GAME_MODULE_END