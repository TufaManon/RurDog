#include "playField.h"
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
		s.flag = this->flag++;
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
		res &= (c.y < 22 && c.x < 10 && c.y >= 0 && c.x >= 0) && (tiled[c.y][c.x].flag == Piece::BRICK_INVALIDE);
	}
	return res;
}

const int MINO_COORD[][8] = {
	{-1, 0, 0, 0, 1, 0, 2, 0},
	{-1, -1, -1, 0, 0, 0, 1, 0},
	{-1, 0, 0, 0, 1, 0, 1, -1},
	{0, -1, 0, 0, 1, -1, 1, 0},
	{-1, 0, 0, 0, 0, -1, 1, -1},
	{-1, 0, 0, -1, 0, 0, 1, 0},
	{-1, -1, 0, -1, 0, 0, 1, 0} };

const RGBA* CYAN_COLOR = new RGBA(Global_Config::config.colors.CYAN);
const RGBA* YELLO_COLOR = new RGBA(Global_Config::config.colors.YELLO);
const RGBA* PURPLE_COLOR = new RGBA(Global_Config::config.colors.PURPLE);
const RGBA* ORANGE_COLOR = new RGBA(Global_Config::config.colors.ORANGE);
const RGBA* BLUE_COLOR = new RGBA(Global_Config::config.colors.BLUE);
const RGBA* GREEN_COLOR = new RGBA(Global_Config::config.colors.GREEN);
const RGBA* RED_COLOR = new RGBA(Global_Config::config.colors.RED);
const RGBA* GREY_COLOR = new RGBA(Global_Config::config.colors.GREY);

Tetrmino* Play_Field::CreateTetrmino()
{
	auto type = generator->GetNextTetrMinoType();
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

void Play_Field::EachTiledInner(std::function<void(Piece&)> fn)
{
	for (int i = 0; i < 22; i++)
		for (int j = 0; j < 10; j++)
			fn(tiled[i][j]);
}

void Play_Field::EachTiledInner(std::function<void(Piece&, int x, int y)> fn)
{
	for (int i = 0; i < 22; i++)
		for (int j = 0; j < 10; j++)
			fn(tiled[i][j], j, i);
}

void Play_Field::EachTiled(std::function<void(const Piece&)> fn) const
{
	for (int i = 0; i < 22; i++)
		for (int j = 0; j < 10; j++)
			fn(tiled[i][j]);
}

void Play_Field::EachTiled(std::function<void(const Piece&, int x, int y)> fn) const
{
	for (int i = 0; i < 22; i++)
		for (int j = 0; j < 10; j++)
			fn(tiled[i][j], j, i);
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

Play_Field::Play_Field() :Signaler({ SIGNAL_LOCKED, SIGNAL_TOUCH_OBSTACLE, SIGNAL_GAME_OVER })
{
	generator = new Random_Generator();
	falling = CreateTetrmino();
	holded = nullptr;
	tiled = new Piece[22][10];
	next_queue = new std::queue<Tetrmino*>();
	received_harm_queue = new std::queue<Harm>();
	emitted_harm_queue = new std::queue<Harm>();
	EachTiledInner([](Piece& p, int x, int y)->void {
		p.relative_coord = Coord(x, y);
	p.color = GREY_COLOR;
		});
	for (int i = 0; i < 4; i++)
		next_queue->push(CreateTetrmino());
}

Play_Field::~Play_Field()
{
	if (falling != nullptr) delete falling;
	if (holded != nullptr) delete holded;
	delete generator;
	delete next_queue;
	delete received_harm_queue;
	delete emitted_harm_queue;
	delete[] tiled;
}

void Play_Field::Lock()
{
	int x = falling->bounding_box.x;
	int y = falling->bounding_box.y;
	for (auto& piece : falling->pieces)
	{
		piece.relative_coord += falling->bounding_box;
		piece.flag = piece.BRICK_VALIDE;
		tiled[piece.relative_coord.y][piece.relative_coord.x] = piece;
	}
	delete falling;

}

void Play_Field::NewFalling()
{
	if (holded != nullptr)
	{
		falling = holded;
	}
	else
	{
		falling = next_queue->front();
		next_queue->pop();
		next_queue->push(CreateTetrmino());
	}
}

bool Play_Field::CheckRowEli(int row)
{
	bool res = true;
	for (int x = 0; x < 10; x++)
	{
		res &= tiled[row][x].flag == Piece::BRICK_VALIDE;
	}
	return res;
}

bool Play_Field::Eli()
{
	std::vector<int> rows;
	for (int y = 21; y >= 2; y--)
	{
		if (CheckRowEli(y))
		{
			rows.push_back(y);
		}
	}

	int i = 0;
	while (i < rows.size())
	{
		int end = -1;
		if (i < rows.size() - 1)
			end = rows[i + 1] - 1;
		for (int col = rows[i] + i; col - 1 - i > end; col--)
		{
			for (int x = 0; x < 10; x++)
			{
				tiled[col][x] = tiled[col - 1 - i][x];
				tiled[col][x].relative_coord = Coord(x, col);
			}
		}
		i++;
	}

	return rows.size() > 0;
}

bool Play_Field::Swap()
{
	if (holded == nullptr)
	{
		holded = falling;
		falling = next_queue->front();
		next_queue->pop();
		return true;
	}
	return false;
}

bool Play_Field::TestingRotate(Tetrmino*& mino, _Ori orientation) const
{
	for (int i = 0; i < 4; i++)
	{
		Tetrmino* smino = mino->Rotate(orientation, i);
		if (IsNotInObstacle(*smino))
		{
			delete mino;
			mino = smino;
			return true;
		}
		delete smino;
		if (mino->type == _Tetrmino_Class::O) break;
	}
	return false;
}

bool Play_Field::Rotate(_Ori orientation)
{
	return  TestingRotate(falling, orientation);
}

bool Play_Field::Move(_Ori orientation)
{
	return TestingMove(falling, orientation);
}

bool Play_Field::TestingMove(Tetrmino*& mino, _Ori orientation) const
{
	Tetrmino* tmino = new Tetrmino(*mino);
	if (orientation == _Ori::L)
		tmino->bounding_box << 1;
	else
		tmino->bounding_box >> 1;
	if (IsNotInObstacle(*tmino))
	{
		delete mino;
		mino = tmino;
		return true;
	}
	delete tmino;
	return false;
}

bool Play_Field::Drop()
{
	int res = TestingDrop(falling);
	if (!res) Emit(this->SIGNAL_TOUCH_OBSTACLE);
	return res;
}

bool Play_Field::TestingDrop(Tetrmino*& mino) const
{
	Tetrmino* tmino = new Tetrmino(*mino);
	tmino->bounding_box ^ 1;
	if (IsNotInObstacle(*tmino))
	{
		delete mino;
		mino = tmino;
		return true;
	}
	delete tmino;
	return false;
}

GAME_MODULE_END