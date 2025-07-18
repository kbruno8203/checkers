# Checkers Game

A complete C++ implementation of the classic American Checkers game using the GE211 game engine. Created Spring 2022. 

## Features

- **Classic Checkers Gameplay**: Full implementation of standard American Checkers rules
- **Custom Board Sizes**: Supports boards from 2x2 up to 8x8
- **Graphical Interface**: Clean, intuitive GUI with visual feedback
- **Move Validation**: Automatic validation of legal moves and jumps
- **King Promotion**: Pieces automatically become kings when reaching the opposite end
- **Multi-Jump Support**: Support for consecutive jumps in a single turn
- **Score Tracking**: Real-time score display showing captured pieces
- **Game Over Detection**: Automatic win condition detection

## Game Rules

- **Objective**: Capture all opponent pieces or block all their moves
- **Movement**: Regular pieces move diagonally forward only. Jumps are mandatory when possible
- **Kings**: Promoted pieces can move diagonally in all directions
- **Capturing**: Jump over opponent pieces to capture them
- **Multiple Jumps**: If multiple jumps are available, they must be taken
- **Winning**: First player to capture 12 opponent pieces wins

## Controls

- **Mouse Click**: Select a piece and then click on a valid move position
- **Visual Feedback**: 
  - Selected pieces are highlighted in gold
  - Available moves are highlighted in gold
  - Current player's turn is indicated by mouse cursor color

## Installation

### Prerequisites

- CMake 3.3 or higher
- C++11 compatible compiler
- GE211 graphics library

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

### Running the Game

```bash
./Checkers
```

## Project Structure

```
checkers/
├── main.cxx          - Entry point
├── controller.hxx    - Game controller (MVC)
├── controller.cxx    - Controller implementation
├── model.hxx         - Game logic and state
├── model.cxx         - Model implementation
├── view.hxx          - UI rendering
├── view.cxx          - View implementation
├── board.hxx         - Game board representation
├── board.cxx         - Board implementation
├── player.hxx        - Player enumeration
├── player.cxx        - Player implementation
├── move.hxx          - Move representation
├── move.cxx          - Move implementation
├── position_set.hxx  - Position set utilities
├── position_set.cxx  - Position set implementation
├── black_king.png    - Black king piece image
├── red_king.png      - Red king piece image
├── metadata.yml      - Submission details
├── WIKI.md           - Comprehensive documentation
└── README.md         - This file

```

## Customization

### Board Size

The game supports custom board sizes by modifying the constructor parameters:

```cpp
Controller controller(6, 6); // 6x6 board
```

### Piece Images

Custom piece images can be used by replacing:
- `black_king.png`
- `red_king.png`


## License

This project is part of an educational assignment and follows standard academic use guidelines.