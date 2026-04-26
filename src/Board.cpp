#include "Board.h"
#include "TextureManager.h"
#include <fstream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Random.h"

Board::Board(int rows, int cols, int mines)
{
	revealed_tiles = 0;
	this->rows = rows;
	this->cols = cols;
	this->mines = mines;
}

int Board::Initialize_PreMade_Board(string file_name)
{	
	revealed_tiles = 0;
	tiles.clear();
	std::ifstream file(file_name);
	char digit;
	int mine_count = 0;

	for (int i = 0; i < rows; i++)
	{
		std::vector<Tile> current_row;
		tiles.push_back(current_row);
		for (int j = 0; j < cols; j++)
		{
			
			file >> digit;
			if (digit == '1')
				mine_count++;
			Tile t(digit);
			tiles[i].push_back(t);
		}
	}
	
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{	
			float x_pos = static_cast<float>(j * 32);
			float y_pos = static_cast<float>(i * 32);
			tiles[i][j].tile_sprite.setPosition(x_pos, y_pos);
			tiles[i][j].overlay_sprite.setPosition(x_pos, y_pos);
			tiles[i][j].bomb_sprite.setPosition(x_pos, y_pos);

		}
	}

	Calc_Adjacent_Mines();
	return mine_count;
}

void Board::generateBombs(int mRow, int mCol)
{
	int mines_added = 0;
	while (mines_added < mines)
	{
		int row = Random::Int(0, rows - 1);
		int col = Random::Int(0, cols - 1);
		if (!tiles[row][col].GetBombStatus() && (row != mRow && col != mCol))
		{
			tiles[row][col].SetBomb();
			mines_added++;
		}
	}
	Calc_Adjacent_Mines();
}

void Board::createBoard()
{
	tiles.clear();
	this->revealed_tiles = 0;
	for (int i = 0; i < rows; i++)
	{
		std::vector<Tile> current_row;
		tiles.push_back(current_row);
		for (int j = 0; j < cols; j++)
		{
			Tile t;
			tiles[i].push_back(t);
		}
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{	
			float x_pos = j * 32.0f;
			float y_pos = i * 32.0f;
			this->tiles[i][j].tile_sprite.setPosition(x_pos, y_pos);
			this->tiles[i][j].overlay_sprite.setPosition(x_pos, y_pos);
			this->tiles[i][j].bomb_sprite.setPosition(x_pos, y_pos);
		}
	}
}



std::vector<std::vector<Tile>>& Board::Get_Tiles()
{
	return this->tiles;
}

void Board::Reveal_Tile(Tile* tile)
{
	if (!tile->GetBombStatus() && !tile->GetFlagStatus() && !tile->revealed)
	{
		if (tile->nearby_mines == 0)
		{
			revealed_tiles++;
			tile->SetTexture("tile_revealed");
			tile->OverlayTexture("tile_revealed");
			tile->revealed = true;
			for (int k = 0; k < tile->adjacentTiles.size(); k++)
			{
				Reveal_Tile(tile->adjacentTiles[k]);
			}
		}

		// Number of nearby mines is not 0, display the number
		else
		{	
			revealed_tiles++;
			tile->SetTexture("tile_revealed");
			tile->revealed = true;
			string num_mines = std::to_string(tile->nearby_mines);
			tile->OverlayTexture("number_" + num_mines);
		}

	}

}

int Board::Get_Reveal_Tiles_Count() const
{
	return revealed_tiles;
}

void Board::Calc_Adjacent_Mines()
{
	int directions[8][2] = {
		{1, 0}, 
		{-1, 0}, 
		{0, 1},  
		{0, -1},
		{-1, -1}, 
		{1, 1},  
		{-1, 1}, 
		{1, -1}  
	};

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{

			for (int k = 0; k < 8; k++)
			{
				int new_i = i + directions[k][0];
				int new_j = j + directions[k][1];

				//Check each surrounding tile around the current tile, and add to vector.
				if (new_i >= 0 && new_i < rows && new_j >= 0 && new_j < cols && !tiles[i][j].GetBombStatus())
				{
					tiles[i][j].adjacentTiles.push_back(&tiles[new_i][new_j]);
					// Increment nearby mines if there is a nearby mine
					if (tiles[new_i][new_j].GetBombStatus())
						tiles[i][j].nearby_mines++;
				}
				
			}

		}
	}

}
