# Puzzle
***
Classic and legendary fun block puzzle game
This game allows you to test your problem-solving skills even further, and with a variety of difficulty levels, it will provide hours of fun for all puzzle-lovers!

# Game process

![gameplay.gif](./gameplay.gif)


# For Developers
### Data access information
- MiniKit Framework is protected by copyright

## Setup
1. Download or clone the source code
2. `cd` into the directory
3. load submodule
```bash
git submodule update --init --recursive
```
4. Build:
```bash
sh build.sh
```
5. Run:
```bash
cd ./build/game
./Puzzle
```

# Project structure

```

CMakeLists.txt
├── Engine
    CMakeLists.txt
    ├── external 
        ├── minikit             # framework
    └── modules
├── external 
    ├── json                # json-lib
├── game                 # main sources / assets
    CMakeLists.txt
    ├── assets              # multimedia assets
    └── src                 # sources
        CMakeLists.txt

```

# Usage
The controls:
* <kbd>←</kbd> and <kbd>→</kbd> : Move the tetromino left and right,
* <kbd>↓</kbd> : Soft drop of the tetromino,
* <kbd>Space</kbd> : Soft drop of the tetromino,
* <kbd>X</kbd> : Rotate (clockwise) the tetromino,
* <kbd>Z</kbd> : Rotate (backward) the tetromino,
* <kbd>P</kbd> : Pause the game,
* <kbd>I</kbd> : Controlling information
* <kbd>Q</kbd> : Exit the game,
