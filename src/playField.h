#pragma once
#include <string>
#include <functional>
#include <queue>
#include <vector>
#include <array>
#include <initializer_list>
#include <SDL.h>
#include <map>
#include <stack>
#include "utils.hpp"
#include "tetrmino.h"
#include "config.hpp"
GAME_MODULE_BEGIN

class Random_Generator
{
	_Tetrmino_Class types[7] = { 
		_Tetrmino_Class::I,
		_Tetrmino_Class::J,
		_Tetrmino_Class::L,
		_Tetrmino_Class::O,
		_Tetrmino_Class::S,
		_Tetrmino_Class::T,
		_Tetrmino_Class::Z
	};

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
class Play_Field : public Signaler
{

protected:
	Tetrmino* falling;
	Tetrmino* holded;
	std::queue<Harm>* received_harm_queue;
	std::queue<Harm>* emitted_harm_queue;
	void EachTiledInner(std::function<void(Piece&)> fn);
	void EachTiledInner(std::function<void(Piece&, int x, int y)> fn);
	bool FallingPiecesCoordsAre(std::function<bool(const Coordinate&)> fn);
	Random_Generator* generator;
	std::queue<Tetrmino*>* next_queue;
	std::array<Coordinate, 4> Get_Field_Coord(const Tetrmino& mino) const;
	bool IsNotInObstacle(const Tetrmino& mino) const;
	Tetrmino* CreateTetrmino();

public:
	static const int SIGNAL_LOCKED = 1 << 0;
	static const int SIGNAL_TOUCH_OBSTACLE = 1 << 1;
	static const int SIGNAL_GAME_OVER = 1 << 2;
	Piece(*tiled)[10];
	void EachTiled(std::function<void(const Piece&)> fn) const;
	void EachTiled(std::function<void(const Piece&, int x, int y)> fn) const;
	std::array<Piece, 4> GetFallingMinoAbsCoord() const;
	const Tetrmino& GetFalling() const
	{
		return *falling;
	}
	Play_Field();
	~Play_Field();
	void Lock();
	void NewFalling();
	bool CheckRowEli(int row);
	bool Eli();
	//swap current tetrmino and hold tetrmino, in the tetrmino guidline, only swapping once from start falling to locked.
	bool Swap();
	//test whether the given tetrmino can rotation for given orientation, if it can rotation, will change the given tetrmino to rotated tetrmino.
	bool TestingRotate(Tetrmino*& mino, _Ori orientation) const;
	bool Rotate(_Ori orientation);
	bool Move(_Ori orientation);
	bool TestingMove(Tetrmino*& mino, _Ori orientation) const;
	bool Drop();
	bool TestingDrop(Tetrmino*& mino) const;
};

GAME_MODULE_END
