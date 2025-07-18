#pragma once

#include "player.hxx"
#include "move.hxx"
#include <ge211.hxx>

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Board
{
public:
    using Dimensions = ge211::Dims<int>;
    using Position = ge211::Posn<int>;
    using Rectangle = ge211::Rect<int>;

    class reference;

private:
    Dimensions dims_;
    Position_set red_;
    Position_set black_;
    Position_set red_kings;
    Position_set black_kings;

public:
    //constructs a board with given dimensions
    explicit Board(Dimensions dims);

    //returns dimensions value passed into constructor
    Dimensions dimensions() const;

    //returns whether given position is in bounds
    bool good_position(Position) const;

    //returns player stored at pos as long as good_position
    Player operator[](Position pos) const;

    // =====================================
    // PUBLIC CONSTRUCTOR & FUNCTION MEMBERS
    // =====================================

    //returns reference to player at pos as long as good position
    reference operator[](Position pos);

    //stores the given player in all the positions in the given position set
    //undefined if any positions in the `Position_set`
    void set_all(Position_set, Player);

    //returns a rectangle containing all the positions of the board. This
    Rectangle all_positions() const;

    static std::vector<Dimensions> const& king_directions();
    static std::vector<Dimensions> const& red_directions();
    static std::vector<Dimensions> const& black_directions();

    // equality for boards.
    friend bool operator==(Board const&, Board const&);

private:
    Player get_(Position where) const;
    void set_(Position where, Player who);
    void bounds_check_(Position where) const;

#ifdef CS211_TESTING
    // When this class is compiled for testing, members of a struct named
    // Test_access will be allowed to access private members of this class.
    friend struct Test_access;
#endif
};

//
// FREE FUNCTIONS FOR WORKING WITH THE CLASS
//

/// Inequality for boards.
bool
operator!=(Board const&, Board const&);

/// Board printing, suitable for debugging.
std::ostream&
operator<<(std::ostream&, Board const&);

//
// HELPER CLASSES
//

/// Class returned by `operator[](Position)` that simulates
/// an assignable reference to a `Posn<int>`. This is what allows
/// you to write
///
///     board[pos] = player;
///
/// to place `player` at `pos`.
///
/// The definition of the class follows this definition of the
/// `Board` class.
class Board::reference
{
    Board& board_;
    Position pos_;

public:
    /// Assigns the value of `that` to the object of `this`.
    reference& operator=(reference const&) noexcept;

    /// Assigns to the object of the reference.
    reference& operator=(Player) noexcept;

    /// Returns the value of the reference.
    operator Player() const noexcept;

private:
    friend class Board;

    reference(Board&, Position) noexcept;
};
