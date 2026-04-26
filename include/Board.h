#pragma once
#include <vector>
#include "Tile.h"

class Board
{
	int rows;
	int cols;
	int mines;
	int revealed_tiles;
	std::vector<std::vector<Tile>> tiles;

	void Calc_Adjacent_Mines();

public:
	Board(int rows, int cols, int mines);

	int Initialize_PreMade_Board(string file_name);

	void createBoard();

	std::vector<std::vector<Tile>>& Get_Tiles();

	void Reveal_Tile(Tile* tile);

	int Get_Reveal_Tiles_Count() const;

	void generateBombs(int mRow, int Mcol);

};

