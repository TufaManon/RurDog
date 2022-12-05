#pragma once
#include <string>
#include <SDL.h>
#include <functional>
#include <queue>
#include <vector>
#include <array>
#include <initializer_list>
#include <SDL.h>
#include <map>
#include <stack>
#include "config.h"
#include "utils.h"
STAGE_MODULE_BEGIN

enum class _Tetrmino_Class
{
	I, O, T, L, J, S, Z
};
#define ALL_Tetrmino_Class \
		{_Tetrmino_Class::I, _Tetrmino_Class::O, _Tetrmino_Class::T, _Tetrmino_Class::L, _Tetrmino_Class::J, _Tetrmino_Class::S, _Tetrmino_Class::Z}

enum class _Tetrmino_State
{
	_0, R, _2, L
};

inline const _Tetrmino_State operator>>(const _Tetrmino_State& state, int i)
{
	auto c = ((int)state + i);
	return c >= 0 ? (_Tetrmino_State)(c % 4) : (_Tetrmino_State)(4 - (c % 4));
}

inline const _Tetrmino_State operator<<(const _Tetrmino_State& state, int i)
{
	auto c = ((int)state - i);
	return c >= 0 ? (_Tetrmino_State)(c % 4) : (_Tetrmino_State)(4 - (c % 4));
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



#define WALL_KICK_DATE(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)\
		{Coord(n1, n2), Coord(n3, n4), Coord(n5, n6), Coord(n7, n8), Coord(n9, n10)}

//旋转的平移测试，将旋转后的坐标减去不同形态的偏移数据之差，来确定是否可以旋转。
// It is the data to Testing how the tetrmino ratate by computing the difference of the different state;
const std::array<Coordinate, 5> JLSTZ_0_OFFSET 
	= WALL_KICK_DATE(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
const std::array<Coordinate, 5> JLSTZ_R_OFFSET 
	= WALL_KICK_DATE(0, 0, +1, 0, +1, +1, 0, -2, +1, -2);
const std::array<Coordinate, 5> JLSTZ_2_OFFSET 
	= WALL_KICK_DATE(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
const std::array<Coordinate, 5> JLSTZ_L_OFFSET 
	= WALL_KICK_DATE(0, 0, -1, 0, -1, +1, 0, -2, -1, -2);

//旋转的平移测试，将旋转后的坐标减去不同形态的偏移数据之差，来确定是否可以旋转。
// It is the data to Testing how the tetrmino ratate by computing the difference of the different state;
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

struct Piece
{
	//It indicates the brick is non-existent
	static const uint8_t BRICK_INVALIDE = 0;

	// It indicates that the brick exists and needs to be calculated as an obstacle
	static const uint8_t BRICK_VALIDE = 1;

	// the color is used by renderer to set draw color of brick
	RGBA color;
	
	//the coord is relative to the playfield wicth is controled by a player or the bounding box;
	Coordinate relative_coord;

	//indicating whether the brick is exists
	uint8_t flag = BRICK_INVALIDE;
};

inline std::array<Coordinate, 5> operator-(const std::array<Coordinate,5>& left, const std::array<Coordinate, 5>& right)
{
	return {left.at(0) - right.at(0), left.at(1) - right.at(1), left.at(2) - right.at(2), left.at(3) - right.at(3), left.at(4) - right.at(4)};
}
struct Tetrmino
{
	// the bounding box different between I or O and S or Z or L or T or  J. the former has 4*4 box and the latter has 3*3 box;
	Coordinate bounding_box;

	// the pieces witch has coordinate relative to the bounding box;
	std::array<Piece,4> pieces;

	_Tetrmino_Class type;

	_Tetrmino_State state;
	
	// set all 4 brick color
	void SetColor(RGBA color);

	void SetColor(HSLA color);

	// set brick of the terimino color by index
	void SetColor(RGBA color, int index);

	void SetColor(HSLA color, int index);

	Tetrmino& operator=(const Tetrmino& right);

	Tetrmino(const Tetrmino& right);

	Tetrmino()
	{
		bounding_box.x = bounding_box.y = 0;
		state = _Tetrmino_State::_0;
		type = _Tetrmino_Class::I;
	}

	std::array<Coordinate,5> GetOffSet(_Tetrmino_State state) const 
	{
		return GetOffsetTable(this->state) - GetOffsetTable(state);
	}
	const Coordinate GetOOffset(_Tetrmino_State state) const
	{
		return GetOOffsetTable(this->state) - GetOOffsetTable(state);
	}

	const Tetrmino Rotate(_Ori ori, int test = 0) const;

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

	const std::array<Coordinate, 5>& GetOffsetTable(_Tetrmino_State state) const 
	{
		if (IS_J(type) || IS_L(type) || IS_S(type) || IS_T(type) || IS_Z(type))
		{
			if (state == _Tetrmino_State::_0) return JLSTZ_0_OFFSET;
			if (state == _Tetrmino_State::R) return JLSTZ_R_OFFSET;
			if (state == _Tetrmino_State::_2) return JLSTZ_2_OFFSET;
			if (state == _Tetrmino_State::L) return JLSTZ_L_OFFSET;
		}
		if (IS_I(type))
		{
			if (state == _Tetrmino_State::_0) return I_0_OFFSET;
			if (state == _Tetrmino_State::R) return I_R_OFFSET;
			if (state == _Tetrmino_State::_2) return I_2_OFFSET;
			if (state == _Tetrmino_State::L) return I_L_OFFSET;
		}
		return I_0_OFFSET;
	}

	const Coordinate GetOOffsetTable(_Tetrmino_State state) const
	{
		if (state == _Tetrmino_State::_0) return O_0_OFFSET;
		if (state == _Tetrmino_State::R) return O_R_OFFSET;
		if (state == _Tetrmino_State::_2) return O_2_OFFSET;
		if (state == _Tetrmino_State::L) return O_L_OFFSET;
		return O_0_OFFSET;
	}

};
using Mino = Tetrmino;

class Random_Generator
{
	_Tetrmino_Class types[7] = ALL_Tetrmino_Class;
	int  top = 0;

	bool TouchTheTop()
	{
		return top == 7;
	}

	void InitTopPoint()
	{
		top = 0;
	}
public:
	_Tetrmino_Class GetNextTetrMinoType();
};

struct FlagFn
{
	int flag = 0;
	std::function<void(void)> fn;
};

class Signaler
{
private:
	std::map<int, std::vector<FlagFn>> map;
	int flag = 0;
protected:
	void Emit(int signal);
	Signaler(std::initializer_list<int> signals);
public:
	Signaler() = delete;
	int ConnectSignal(int signal, std::function<void(void)> fn);
	void DisconnectSignal(int signal, int flag);
};

struct Harm
{

};

class Stage
{
private:
	struct Task
	{
		std::function<bool(uint64_t)> fn;
		uint64_t begin;
		uint64_t duratin;
	};
public:
	std::vector<Task> tasks{};
	Stage()
	{
		if (!SDL_WasInit(SDL_INIT_TIMER))
		{
			SDL_Log(WARNNING,"the timer subsystem hadn't been inited when the stage class init");
		}
	}

	void BlockingAct(std::function<bool(uint64_t)> fn, uint64_t duration)
	{
		auto time = SDL_GetTicks64() + duration;
		while (SDL_GetTicks64() < time)
		{
			if(fn(SDL_GetTicks64() - time)) return;
		}
	}	

	void UnblockingAct(std::function<bool(uint64_t)> fn, uint64_t duration)
	{
		Task task{ fn,SDL_GetTicks64(),duration };
		tasks.push_back(task);
	}

	virtual void UpdateContent() = 0;

	void Update()
	{
		for (auto it = tasks.begin(); it != tasks.end();)
		{
			if ((*it).fn(SDL_GetTicks64() - (*it).begin))
			{
				it = tasks.erase(it);
			}
			else
			{
				it++;
			}
		}

		UpdateContent();
	}
};

class Play_Field: public Signaler
{
	Random_Generator generator;
	Tetrmino* falling;
	Tetrmino* hold;
	std::queue<Harm> received_harm_queue;
	std::queue<Harm> emitted_harm_queue;
	std::queue<Tetrmino*> next_queue;
	bool game_over = false;
	std::array<Coordinate, 4> Get_Field_Coord(const Tetrmino& mino) const;
	bool IsNotInObstacle(const Tetrmino& mino) const;
	Tetrmino* CreateTetrmino();
	bool FallingPiecesCoordsAre(std::function<bool(const Coordinate&)> fn);

public:
	static const int SIGNAL_LOCKED = 1 << 0;
	static const int SIGNAL_TOUCH_OBSTACLE = 1 << 1;
	static const int SIGNAL_GAME_OVER = 1 << 2;
	Piece field_backs[22][10];
	void EacBacks(std::function<void(const Piece&)> fn);
	std::array<Piece,4> GetFallingMinoAbsCoord() const;
	Play_Field();
	void Locking();
	//swap current tetrmino and hold tetrmino, in the tetrmino guidline, only swapping once from start falling to locked.
	void Swaping();
	//test whether the given tetrmino can rotation for given orientation, if it can rotation, will change the given tetrmino to rotated tetrmino.
	bool TestingRotation(Tetrmino& mino, _Ori orientation);
	bool Rotation(_Ori orientation);
	bool Moving(_Ori orientation);
	bool TestingMoving(Tetrmino& mino,_Ori orientation);
	bool Drop();
	bool TestingDrop(Tetrmino& mino);
	bool IsGameOver()
	{
		return game_over;
	}
};

STAGE_MODULE_END
