#include "Game.h"
using RurDog::Game;

int main(int argc, int arcs)
{
	Game::GetGame().Start();
	Game::GetGame().Destory();
	return 0;
}