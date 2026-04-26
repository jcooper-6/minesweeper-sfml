#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

class Tile
{
	bool is_bomb;
	bool is_flagged;

public:
	bool revealed;
	sf::Sprite tile_sprite;
	sf::Sprite overlay_sprite;
	sf::Sprite bomb_sprite;
	std::vector<Tile*> adjacentTiles;
	int nearby_mines;

	Tile(char value);

	Tile();

	void SetFlagged(bool val);

	bool GetFlagStatus() const;

	bool GetBombStatus() const;

	void SetTexture(string name);

	void OverlayTexture(string name);

	void SetBomb();
};

