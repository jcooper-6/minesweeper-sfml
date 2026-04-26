
# Minesweeper (C++ / SFML)

This is a minesweeper project built using SFML-2.6.1 64bit. It was originally created as the final project for COP3503C - Programming Fundamentals 2.



## Features

- Classic Minesweeper gameplay
- Flagging system for marking mines
- Custom board loading from files
- Debug mode for showing mines
- UI built using SFML


## Tech Stack

- C++ 

- SFML-2.6.1 64bit

- Visual Studio

## Build Instructions

- Clone the repository
- Download SFML-2.6.1 64bit
- Open the Visual Studio project file and configure:
    - All configurations
        - Include directories -> SFML-2.6.1\include
        - Library directories -> SFML-2.6.1\lib
    - Release mode:
        - Additional Dependencies -> sfml-graphics.lib;sfml-window.lib;sfml-system.lib
    - Debug mode:
        - Additional Dependencies -> sfml-graphics-d.lib;sfml-window-d.lib;sfml-system-d.lib
- Make sure to include the dll files from SFML-2.6.1\bin where your compiled program .exe is located.
## How it Works

- The board is generated from files containing ones(mines) and zeros(empty)
- Each tile object tracks its state and adjacent mine count.
- Recursive logic reveals the empty regions.

## Future Improvements
- Animations
- Difficulty levels
- Sound effects
- Cross-platform CMake build system
## Author

- [@jcooper-6](https://github.com/jcooper-6)

