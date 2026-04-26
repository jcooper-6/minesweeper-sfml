#include "Tile.h"
#include <iostream>

Tile::Tile(char value)
{
	
	if (value == '1') {

		this->is_bomb = true;
	}
	else if (value == '0')
	{
		this->is_bomb = false;
	}
	else
	{
		std::cout << "Error in creating tile" << std::endl;
		this->is_bomb = false;
	}
		
	nearby_mines = 0;
	revealed = false;
	is_flagged = false;
	tile_sprite.setTexture(TextureManager::GetTexture("tile_hidden"));
	overlay_sprite.setTexture(TextureManager::GetTexture("tile_hidden"));
	bomb_sprite.setTexture(TextureManager::GetTexture("mine"));



}

Tile::Tile()
{
	nearby_mines = 0;
	tile_sprite.setTexture(TextureManager::GetTexture("tile_hidden"));
	overlay_sprite.setTexture(TextureManager::GetTexture("tile_hidden"));
	bomb_sprite.setTexture(TextureManager::GetTexture("mine"));
	revealed = false;
	is_flagged = false;
	is_bomb = false;
}

void Tile::SetFlagged(bool val)
{
	if (val)
		is_flagged = true;
	else
		is_flagged = false;
}

bool Tile::GetFlagStatus() const
{
	return is_flagged;
}

bool Tile::GetBombStatus() const
{
	return is_bomb;
}

void Tile::SetTexture(string name)
{
	tile_sprite.setTexture(TextureManager::GetTexture(name));
}

void Tile::OverlayTexture(string name)
{
	overlay_sprite.setTexture(TextureManager::GetTexture(name));
}

void Tile::SetBomb()
{
	this->is_bomb = true;
}
