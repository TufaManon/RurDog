#include "Game.h"
using RurDog::Game;

int main(int argc, int arcs)
{
	Game::GetGame().Start();
	Game::Destory();
	return 0;
}