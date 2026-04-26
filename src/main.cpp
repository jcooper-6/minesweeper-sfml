#include <SFML/Graphics.hpp> 
#include <iostream>
#include "TextureManager.h"
#include "Random.h"
#include <fstream>
#include "Board.h"

/* ----------------------------------------------------------------------------------------------------------------------
    This project uses SFML-2.6.1 64-bit, and was created as the final project for COP3503C - Programming Fundamentals 2
   ----------------------------------------------------------------------------------------------------------------------
*/

static std::vector<sf::Sprite> display_flags_remaining(int flags, int mine_count, int rows, int cols, int grid_size)
{   
    int count = mine_count - flags;
    std::vector<sf::Sprite> sprites;
    float sprite_y_pos = static_cast<float>(rows * grid_size);

    sf::Sprite first_num;
    first_num.setTexture(TextureManager::GetTexture("digits"));
    first_num.setPosition(21.0f, sprite_y_pos);

    sf::Sprite second_num;
    second_num.setTexture(TextureManager::GetTexture("digits"));
    second_num.setPosition(42.0f, sprite_y_pos);

    sf::Sprite third_num;
    third_num.setTexture(TextureManager::GetTexture("digits"));
    third_num.setPosition(63.0f, sprite_y_pos);

    first_num.setTextureRect(sf::IntRect(0, 0, 21, 32));
    second_num.setTextureRect(sf::IntRect(0, 0, 21, 32));
    third_num.setTextureRect(sf::IntRect(0, 0, 21, 32));



    if (count >= 0)
    {
        int first_digit = count / 100;
        int second_digit = (count / 10) % 10;
        int third_digit = count % 10;

        first_num.setTextureRect(sf::IntRect(first_digit * 21, 0, 21, 32));
        second_num.setTextureRect(sf::IntRect(second_digit * 21, 0, 21, 32));
        third_num.setTextureRect(sf::IntRect(third_digit * 21, 0, 21, 32));
    }
    else if (count < 0)
    {
        sf::Sprite sign;
        sign.setTexture(TextureManager::GetTexture("digits"));
        sign.setPosition(0.0f, sprite_y_pos);
        sign.setTextureRect(sf::IntRect(21 * 10, 0, 21, 32));
        sprites.push_back(sign);

        int first_digit = -1 * count / 100;
        int second_digit = -1 *  (count / 10) % 10;
        int third_digit = -1 * count % 10;

        first_num.setTextureRect(sf::IntRect(first_digit * 21, 0, 21, 32));
        second_num.setTextureRect(sf::IntRect(second_digit * 21, 0, 21, 32));
        third_num.setTextureRect(sf::IntRect(third_digit * 21, 0, 21, 32));
    }

    sprites.push_back(first_num);
    sprites.push_back(second_num);
    sprites.push_back(third_num);
    return sprites;

}

static bool mouse_button_collison(sf::Sprite button, float mouse_pos_x, float mouse_pos_y)
{
    if (mouse_pos_y > button.getGlobalBounds().top && mouse_pos_y < (button.getGlobalBounds().top + button.getGlobalBounds().height) && mouse_pos_x > button.getGlobalBounds().left && mouse_pos_x < (button.getGlobalBounds().left + button.getGlobalBounds().width))
        return true;
    else
        return false;
}

static bool mouse_in_bounds(int rows, int cols, sf::Vector2f mouse_pos_grid)
{
    if (mouse_pos_grid.x < cols && mouse_pos_grid.x >= 0 && mouse_pos_grid.y >= 0 && mouse_pos_grid.y < rows)
        return true;
    else
        return false;
}

static void update_mouse_pos(sf::Vector2f &mouse_pos, sf::Vector2f &mouse_pos_grid, sf::Window &window, int &grid_size)
{
    sf::Vector2i raw_mouse_pos = sf::Mouse::getPosition(window);
    float f_grid_size = static_cast<float>(grid_size);
    mouse_pos_grid.x = raw_mouse_pos.x / f_grid_size;
    mouse_pos_grid.y = raw_mouse_pos.y / f_grid_size;
    mouse_pos.x = static_cast<float>(raw_mouse_pos.x);
    mouse_pos.y = static_cast<float>(raw_mouse_pos.y);
}

static void handle_game_loss(Board& game_board, sf::Sprite& smile)
{
    for (auto& row : game_board.Get_Tiles())
    {
        for (auto& tile : row)
        {
            if (tile.GetBombStatus())
            {
                tile.SetBomb();
                tile.revealed = true;
                tile.SetTexture("tile_revealed");
                tile.OverlayTexture("mine");
            }
        }
    }
    smile.setTexture(TextureManager::GetTexture("face_lose"));
}

static void handle_game_win(Board& game_board, sf::Sprite& smile)
{
    // Mark any unrevealed tiles with flags
    for (auto& row : game_board.Get_Tiles())
    {
        for (auto& tile : row)
        {
            if (!tile.revealed)
            {
                tile.SetFlagged(1);
                tile.revealed = true;
                tile.OverlayTexture("flag");
            }
        }
    }
    smile.setTexture(TextureManager::GetTexture("face_win"));
}

int main()
{   

    // Variables
    int grid_size = 32;
    int flags_placed = 0;
    int cols;
    int rows;
    int mine_count;
    int original_mine_count;

        // Bools
        bool debug_on = false;
        bool game_lost = false;
        bool game_won = false;
        bool first_click = true;

        // Mouse Position Variables
        sf::Vector2f mouse_pos_grid;
        sf::Vector2f mouse_pos;

    // Get Board Config
    std::ifstream cfg("boards/config.cfg");

    if (cfg.is_open())
    {
        string line;
   
        getline(cfg, line);
        cols = std::stoi(line);

        getline(cfg, line);
        rows = std::stoi(line);
       
        getline(cfg, line);
        mine_count = std::stoi(line);

    }

    else 
    {
        std::cout << "Failed to open config.cfg" << std::endl;
        return 1;
    }
    original_mine_count = mine_count;

    int screen_width = cols * 32;
    int screen_height = (rows * 32) + 100;

    // Create Board

    Board game_board(rows, cols, mine_count);

    game_board.createBoard();


    // Create Window
    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "Minesweeper");
    window.setFramerateLimit(60);
    // ========================================
    // ============  Creating Sprites  
    // ========================================


    sf::Sprite smile;
    smile.setTexture(TextureManager::GetTexture("face_happy"));

    float sprite_width = smile.getGlobalBounds().width;
    float sprite_y_pos = static_cast<float>(rows * grid_size);

    smile.setPosition((((cols * grid_size) / 2.0f) - (sprite_width / 2.0f)), sprite_y_pos);

    sf::Sprite bomb_icon;
    bomb_icon.setTexture(TextureManager::GetTexture("debug"));
    bomb_icon.setPosition(screen_width - 4 * sprite_width, sprite_y_pos);

    sf::Sprite test_1;
    test_1.setTexture(TextureManager::GetTexture("test_1"));
    test_1.setPosition(screen_width - 3 * sprite_width, sprite_y_pos);
    
    sf::Sprite test_2;
    test_2.setTexture(TextureManager::GetTexture("test_2"));
    test_2.setPosition(screen_width - 2 * sprite_width, sprite_y_pos);

    sf::Sprite test_3;
    test_3.setTexture(TextureManager::GetTexture("test_3"));
    test_3.setPosition(screen_width - sprite_width, sprite_y_pos);


    // ========================================
    // ============  Game Loop  
    // ========================================

    while (window.isOpen())
    {
        sf::Event event;

        // Update Mouse Position
        update_mouse_pos(mouse_pos, mouse_pos_grid, window, grid_size);

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && mouse_in_bounds(rows, cols, mouse_pos_grid) && !game_lost && !game_won)
            {
                int grid_x = static_cast<int>(mouse_pos_grid.x);
                int grid_y = static_cast<int>(mouse_pos_grid.y);

                Tile& tile = game_board.Get_Tiles()[grid_y][grid_x];

                if (!tile.revealed)
                {
                    bool currentlyFlagged = tile.GetFlagStatus();

                    if (!currentlyFlagged)
                    {
                        tile.SetFlagged(true);
                        tile.OverlayTexture("flag");
                        flags_placed++;
                    }

                    else if (currentlyFlagged)
                    {
                        tile.SetFlagged(false);
                        tile.OverlayTexture("tile_hidden");
                        flags_placed--;
                    }
                }
                


            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {

                // Code for clicking the debug button ----------------------------
                if (mouse_button_collison(bomb_icon, mouse_pos.x, mouse_pos.y) && !game_lost && !game_won)
                {
                    debug_on = !debug_on;
                }

                // Code for clicking the smiley face ----------------------------
                if (mouse_button_collison(smile, mouse_pos.x, mouse_pos.y))
                {
                    flags_placed = 0;
                    mine_count = original_mine_count;
                    game_board.createBoard();
                    smile.setTexture(TextureManager::GetTexture("face_happy"));
                    game_won = false;
                    game_lost = false;
                    first_click = true;
                }
                // Code for clicking Test_1
                if (mouse_button_collison(test_1, mouse_pos.x, mouse_pos.y))
                {
                    flags_placed = 0;
                    mine_count = game_board.Initialize_PreMade_Board("boards/testboard1.brd");
                    smile.setTexture(TextureManager::GetTexture("face_happy"));
                    game_won = false;
                    game_lost = false;
                }
                // Code for clicking Test_2
                if (mouse_button_collison(test_2, mouse_pos.x, mouse_pos.y))
                {
                    flags_placed = 0;
                    mine_count = game_board.Initialize_PreMade_Board("boards/testboard2.brd");
                    smile.setTexture(TextureManager::GetTexture("face_happy"));
                    game_won = false;
                    game_lost = false;
                }
                // Code for clicking Test_3
                if (mouse_button_collison(test_3, mouse_pos.x, mouse_pos.y))
                {
                    flags_placed = 0;
                    mine_count = game_board.Initialize_PreMade_Board("boards/testboard3.brd");
                    smile.setTexture(TextureManager::GetTexture("face_happy"));
                    game_won = false;
                    game_lost = false;
                }

                // Code for Selecting a Tile to Reveal ----------------------------
                if (mouse_in_bounds(rows, cols, mouse_pos_grid) && !game_lost && !game_won)
                {
                    int grid_x = static_cast<int>(mouse_pos_grid.x);
                    int grid_y = static_cast<int>(mouse_pos_grid.y);

                    if (first_click)
                    {
                        game_board.generateBombs(grid_y, grid_x);
                        first_click = false;
                    }

                    // Handle Game Over
                    if (game_board.Get_Tiles()[grid_y][grid_x].GetBombStatus()) 
                    {
                        game_lost = true;
                        handle_game_loss(game_board, smile);
                    }
                    else
                    {
                        game_board.Reveal_Tile(&game_board.Get_Tiles()[grid_y][grid_x]);

                        // Handle Game Win
                        if (game_board.Get_Reveal_Tiles_Count() == ((rows * cols) - mine_count))
                        {
                            flags_placed = mine_count;
                            debug_on = false;
                            game_won = true;
                            handle_game_win(game_board, smile);
                        }
                    }
                }

            }
        }



        // Flags Remaining Counter ----------------------------
        std::vector<sf::Sprite> flag_counter = display_flags_remaining(flags_placed, mine_count, rows, cols, grid_size);

        window.clear(sf::Color(255, 255, 255));
        // Draw tiles
        for (auto& row : game_board.Get_Tiles())
        {
            for (auto& tile : row)
            {
                window.draw(tile.tile_sprite);
                window.draw(tile.overlay_sprite);
            }

        }
        // Draw Flag Counter ----------------------------
        for (int i = 0; i < flag_counter.size(); i++)
        {
            window.draw(flag_counter[i]);
        }

        // Draw mines if debug is on ----------------------------
        if (debug_on)
        {
            for (auto& row : game_board.Get_Tiles())
            {
                for (auto& tile : row)
                {
                    if (tile.GetBombStatus())
                        window.draw(tile.bomb_sprite);
                }

            }
        }

        
        // Draw the rest of GUI
        window.draw(smile);
        window.draw(bomb_icon);
        window.draw(test_1);
        window.draw(test_2);
        window.draw(test_3);

        window.display();

        
    }

    return 0;
}
