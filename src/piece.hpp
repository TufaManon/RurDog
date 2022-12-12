#include "color.h"	
#include "utils.hpp"
#include "coordinate.hpp"
GAME_MODULE_BEGIN
struct Piece
{
	//It indicates the brick is non-existent
	static const uint8_t BRICK_INVALIDE = 0;

	// It indicates that the brick exists and needs to be calculated as an obstacle
	static const uint8_t BRICK_VALIDE = 1;

	// the color is used by renderer to set draw color of brick
	const RGBA* color;

	//the coord is relative to the playfield wicth is controled by a player or the bounding box;
	Coordinate relative_coord;

	//indicating whether the brick is exists
	uint8_t flag = BRICK_INVALIDE;
};
GAME_MODULE_END