# Checkers Game - Code Documentation Wiki

This wiki provides comprehensive documentation of all classes, functions, and their purposes in the Checkers game implementation.

## Table of Contents

1. [Model Class](#model-class)
2. [Board Class](#board-class)
3. [View Class](#view-class)
4. [Controller Class](#controller-class)
5. [Player Enum and Functions](#player-enum-and-functions)
6. [Position_set Class](#position_set-class)
7. [Move Types](#move-types)
8. [Main Function](#main-function)

---

## Model Class

The `Model` class contains the core game logic and state management for the Checkers game.

### Public Members

#### Constructors
- **`Model(int size = 8)`**: Creates a square board with the specified size (default 8x8)
- **`Model(int width, int height)`**: Creates a rectangular board with specified dimensions

#### Public Data Members
- **`Position_set poss_moves`**: Set of possible moves for the currently selected piece
- **`Position sel_posn`**: Currently selected position on the board
- **`int scores[2]`**: Array storing scores for both players [red_score, black_score]

#### Public Methods
- **`Rectangle board() const`**: Returns the rectangle representing the entire board
- **`bool is_game_over() const`**: Returns true if the game has ended
- **`Player turn() const`**: Returns the current player's turn or `Player::neither` if game over
- **`Player winner() const`**: Returns the winning player or `Player::neither` if no winner
- **`Player operator[](Position) const`**: Returns the player at the given position
- **`void play_move(Position)`**: Attempts to play a move at the given position

### Private Members

#### Private Data Members
- **`Player turn_`**: Current player's turn (initialized to `Player::black`)
- **`Player winner_`**: Game winner (initialized to `Player::neither`)
- **`Board board_`**: The game board instance
- **`Move_map next_moves_`**: Map of next possible moves
- **`Move_map mm`**: Move map for tracking jumps

#### Private Helper Methods
- **`bool is_opp(Player p) const`**: Returns true if the given player is the opponent of the current turn
- **`Position_set check_jumps_(Position current)`**: Calculates all possible moves (including jumps) from a given position
- **`void advance_turn_()`**: Changes turn to the next player and checks for game over conditions
- **`void set_game_over_()`**: Ends the game and determines the winner
- **`void really_play_move_(Position played)`**: Executes a validated move, updating board state and scores

---

## Board Class

The `Board` class manages the physical representation of the checkers board and piece positions.

### Public Members

#### Type Aliases
- **`Dimensions`**: Alias for `ge211::Dims<int>`
- **`Position`**: Alias for `ge211::Posn<int>`
- **`Rectangle`**: Alias for `ge211::Rect<int>`

#### Constructor
- **`Board(Dimensions dims)`**: Creates a board with specified dimensions

#### Public Methods
- **`Dimensions dimensions() const`**: Returns the board dimensions
- **`bool good_position(Position) const`**: Checks if a position is within board bounds
- **`Player operator[](Position pos) const`**: Returns the player at the specified position (read-only)
- **`reference operator[](Position pos)`**: Returns a reference for modifying the player at a position
- **`void set_all(Position_set, Player)`**: Sets all positions in the set to the specified player
- **`Rectangle all_positions() const`**: Returns a rectangle containing all board positions

#### Static Direction Methods
- **`static std::vector<Dimensions> const& king_directions()`**: Returns movement directions for king pieces (all four diagonals)
- **`static std::vector<Dimensions> const& red_directions()`**: Returns movement directions for red pieces (forward diagonals)
- **`static std::vector<Dimensions> const& black_directions()`**: Returns movement directions for black pieces (backward diagonals)

### Private Members
- **`Dimensions dims_`**: Board dimensions
- **`Position_set red_`**: Set of positions containing red pieces
- **`Position_set black_`**: Set of positions containing black pieces
- **`Position_set red_kings`**: Set of positions containing red king pieces
- **`Position_set black_kings`**: Set of positions containing black king pieces

#### Private Helper Methods
- **`Player get_(Position where) const`**: Internal method to get player at position
- **`void set_(Position where, Player who)`**: Internal method to set player at position
- **`void bounds_check_(Position where) const`**: Validates position is within bounds

### Helper Classes

#### Board::reference Class
Allows assignment syntax like `board[pos] = player`

- **`reference& operator=(reference const&)`**: Copy assignment
- **`reference& operator=(Player)`**: Assigns a player to the position
- **`operator Player() const`**: Converts reference to Player value

---

## View Class

The `View` class handles all graphics rendering and visual representation of the game.

### Public Members

#### Type Aliases
- **`Dimensions`**: Alias for `ge211::Dims<int>`
- **`Position`**: Alias for `ge211::Posn<int>`
- **`Rectangle`**: Alias for `ge211::Rect<int>`

#### Constructor
- **`View(Model const& model)`**: Creates a view associated with the given model

#### Public Methods
- **`void draw(ge211::Sprite_set& set, Position)`**: Main drawing method that renders the entire game state
- **`Dimensions initial_window_dimensions() const`**: Returns the initial window size
- **`std::string initial_window_title() const`**: Returns the window title ("Checkers")

#### Static Utility Methods
- **`static Position board_to_screen(Model::Position board_pos)`**: Converts board coordinates to screen coordinates
- **`static Model::Position screen_to_board(Position screen_pos)`**: Converts screen coordinates to board coordinates
- **`Model::Position center_circle(Position pos) const`**: Centers a circular sprite within a grid square

### Private Members

#### Sprites and Graphics
- **`ge211::Font sans18`**: Font for text rendering
- **`ge211::Text_sprite red_score_label, black_score_label`**: Score display labels
- **`ge211::Text_sprite red_wins_output, black_wins_output`**: Game over messages
- **Various sprite objects**: For board tiles, pieces, kings, and mouse cursor

#### Private Helper Methods
- **`void add_player_sprite_(ge211::Sprite_set&, Player, Position, int z) const`**: Adds a player piece sprite to the display
- **`void add_mouse_(ge211::Sprite_set&, Player, Position, int z) const`**: Adds mouse cursor sprite

---

## Controller Class

The `Controller` class manages user input and coordinates between Model and View.

### Public Members

#### Constructors
- **`Controller(int size = 8)`**: Creates controller with square board
- **`Controller(int width, int height)`**: Creates controller with rectangular board

#### Inherited Methods (from ge211::Abstract_game)
- **`void draw(ge211::Sprite_set& set) override`**: Main drawing callback
- **`View::Dimensions initial_window_dimensions() const override`**: Returns window dimensions
- **`std::string initial_window_title() const override`**: Returns window title

#### Event Handlers
- **`void on_mouse_move(ge211::Posn<int>) override`**: Handles mouse movement
- **`void on_mouse_down(ge211::events::Mouse_button, ge211::Posn<int>) override`**: Handles mouse clicks

### Private Members
- **`Model model_`**: The game model instance
- **`View view_`**: The view instance
- **`ge211::Posn<int> mouse_posn`**: Current mouse position

---

## Player Enum and Functions

### Player Enum
```cpp
enum class Player {
    black,      // Regular black piece
    red,        // Regular red piece
    black_king, // Black king piece
    red_king,   // Red king piece
    neither     // Empty position or no player
};
```

### Free Functions
- **`Player other_player(Player)`**: Returns the opposite player (black ↔ red)
- **`std::ostream& operator<<(std::ostream&, Player)`**: Prints player representation (R, B, RK, BK, N)

---

## Position_set Class

A highly optimized class for managing sets of board positions using bitsets.

### Public Members

#### Constants
- **`static constexpr int coord_limit = 8`**: Maximum coordinate value (exclusive)

#### Type Aliases
- **`value_type`**: Alias for `ge211::Posn<int>`

#### Constructors
- **`Position_set()`**: Default constructor (empty set)
- **`Position_set(std::initializer_list<value_type> elems)`**: Construct from position list
- **`Position_set(FWD_ITER begin, FWD_ITER end)`**: Construct from iterator range

#### Static Factory
- **`static Position_set universe()`**: Returns set of all possible positions

#### Basic Operations
- **`bool empty() const`**: Returns true if set is empty
- **`std::size_t size() const`**: Returns number of positions in set
- **`bool operator[](value_type p) const`**: Tests if position is in set
- **`reference operator[](value_type p)`**: Returns reference for modifying membership
- **`void clear()`**: Removes all positions from set

#### Set Operations
- **`Position_set& operator&=(Position_set that)`**: Intersection assignment
- **`Position_set& operator|=(Position_set that)`**: Union assignment
- **`Position_set& operator^=(Position_set that)`**: Symmetric difference assignment
- **`Position_set operator&(Position_set) const`**: Intersection
- **`Position_set operator|(Position_set) const`**: Union
- **`Position_set operator^(Position_set) const`**: Symmetric difference
- **`Position_set operator~() const`**: Complement

#### Iterator Support
- **`iterator begin() const`**: Returns iterator to first position
- **`iterator end() const`**: Returns past-the-end iterator

### Position_set::iterator Class
Bidirectional iterator for traversing positions in the set.

- **`value_type operator*() const`**: Dereference to get current position
- **`value_type* operator->() const`**: Arrow operator
- **`iterator& operator++()`**: Pre-increment
- **`iterator& operator--()`**: Pre-decrement
- **`iterator operator++(int)`**: Post-increment
- **`iterator operator--(int)`**: Post-decrement

---

## Move Types

### Move Type
```cpp
using Move = std::pair<ge211::Posn<int> const, Position_set>;
```
Represents a move as a pair of:
- **first**: The position where the move is played
- **second**: All positions affected by the move (including the played position)

### Move_map Type
```cpp
using Move_map = std::unordered_map<ge211::Posn<int>, Position_set>;
```
Maps move positions to their corresponding change sets for efficient move lookup.

### Free Functions
- **`std::ostream& operator<<(std::ostream&, Move const&)`**: Prints move information

---

## Main Function

### main()
```cpp
int main() {
    Controller().run();
    return 0;
}
```

The entry point of the program:
1. Creates a `Controller` instance with default parameters (8x8 board)
2. Calls `run()` to start the game loop
3. Returns 0 on successful completion

---

## Key Design Patterns

### Model-View-Controller (MVC)
- **Model**: Manages game state and logic
- **View**: Handles rendering and visual representation
- **Controller**: Processes user input and coordinates between Model and View

### Efficient Data Structures
- **Position_set**: Uses bitsets for O(1) set operations on board positions
- **Move_map**: Hash map for fast move lookup and validation

### RAII and Resource Management
- Automatic resource management through constructors and destructors
- Safe memory handling with standard containers

This documentation provides a complete reference for understanding and maintaining the Checkers game codebase.
