#include "tetrmino.h"
GAME_MODULE_BEGIN

const std::array<Coordinate, 5> JLSTZ_0_OFFSET
= WALL_KICK_DATE(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
const std::array<Coordinate, 5> JLSTZ_R_OFFSET
= WALL_KICK_DATE(0, 0, +1, 0, +1, +1, 0, -2, +1, -2);
const std::array<Coordinate, 5> JLSTZ_2_OFFSET
= WALL_KICK_DATE(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
const std::array<Coordinate, 5> JLSTZ_L_OFFSET
= WALL_KICK_DATE(0, 0, -1 ,0, -1, +1, 0, -2, -1, -2);

const std::array<Coordinate, 5> I_0_OFFSET
= WALL_KICK_DATE(0, 0, -1, 0, +2, 0, -1, 0, +2, 0);
const std::array<Coordinate, 5> I_R_OFFSET
= WALL_KICK_DATE(-1, 0, 0, 0, 0, 0, 0, -1, 0, +2);
const std::array<Coordinate, 5> I_2_OFFSET
= WALL_KICK_DATE(-1, -1, +1, -1, -2, -1, +1, 0, -2, 0);
const std::array<Coordinate, 5> I_L_OFFSET
= WALL_KICK_DATE(0, -1, 0, -1, 0, -1, 0, +1, 0, -2);
const Coordinate O_0_OFFSET = Coord(0, 0);
const Coordinate O_R_OFFSET = Coord(0, +1);
const Coordinate O_2_OFFSET = Coord(-1, +1);
const Coordinate O_L_OFFSET = Coord(-1, 0);

void Tetrmino::SetColor(const RGBA* color)
{
	for (int i = 0; i < pieces.size(); i++)
	{
		pieces.at(i).color = color;
	}
}

Tetrmino* Tetrmino::Rotate(_Ori ori, int test) const
{
	auto s = GetNextState(ori);
	Tetrmino* mino = new Tetrmino(*this);
	mino->state = s;
	mino->CenterRotation(ori);
	Coordinate offset = type == _Tetrmino_Class::O ? GetOOffset(s) : GetOffSet(s).at(test);
	mino->bounding_box = bounding_box + offset;
	return mino;
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
const std::array<Coordinate, 5>& Tetrmino::GetOffsetTable(_Tetrmino_State state) const
{
	if (this->type == _Tetrmino_Class::J ||
		this->type == _Tetrmino_Class::L ||
		this->type == _Tetrmino_Class::S ||
		this->type == _Tetrmino_Class::T ||
		this->type == _Tetrmino_Class::Z 
		)
	{
		switch (state)
		{
		case RurDog::_Tetrmino_State::_0:
			return JLSTZ_0_OFFSET;
			break;
		case RurDog::_Tetrmino_State::R:
			return JLSTZ_R_OFFSET;
			break;
		case RurDog::_Tetrmino_State::_2:
			return JLSTZ_2_OFFSET;
			break;
		case RurDog::_Tetrmino_State::L:
			return JLSTZ_L_OFFSET;
			break;
		default:
			break;
		}
	}

	if(this->type == _Tetrmino_Class::I)
		switch (state)
		{
		case RurDog::_Tetrmino_State::_0:
			return I_0_OFFSET;
			break;
		case RurDog::_Tetrmino_State::R:
			return I_R_OFFSET;
			break;
		case RurDog::_Tetrmino_State::_2:
			return I_2_OFFSET;
			break;
		case RurDog::_Tetrmino_State::L:
			return I_L_OFFSET;
			break;
		default:
			break;
		}
	
	return std::array<Coordinate, 5>();
}
const Coordinate Tetrmino::GetOOffsetTable(_Tetrmino_State state) const
{
	switch (state)
	{
	case RurDog::_Tetrmino_State::_0:
		return O_0_OFFSET;
		break;
	case RurDog::_Tetrmino_State::R:
		return O_R_OFFSET;
		break;
	case RurDog::_Tetrmino_State::_2:
		return O_2_OFFSET;
		break;
	case RurDog::_Tetrmino_State::L:
		return O_L_OFFSET;
		break;
	}
}
GAME_MODULE_END