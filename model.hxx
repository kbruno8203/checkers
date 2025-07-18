#pragma once

#include <ge211.hxx>
#include "player.hxx"
#include "board.hxx"
#include <ge211.hxx>
#include <iostream>
#include <vector>

static
Move_map mm = Move_map();

class Model
{
public:
    using Position = Board::Position;

    using Rectangle = Board::Rectangle;

    Position_set poss_moves;

    Position sel_posn;

    //standard checkers game is 8x8
    explicit Model(int size = 8);

    //cant be <2 or >8
    Model(int width, int height);

    Rectangle board() const;

    // returns whether the game is finished.
    bool is_game_over() const
    {return turn() == Player::neither;}

    // returns the current turn, or `Player::neither` if the game is over
    Player turn() const
    { return turn_; }

    // returns the winner, or `Player::neither` if there is no winner
    Player winner() const
    { return winner_; }

    // Returns the player at the given position, or `Player::neither` if
    // the position is unoccupied; position cant be out of bounds
    Player operator[](Position) const;

    /// Attempts to play a move at the given position for the current
    /// player. If successful, advances the state of the game to the
    /// correct player or game over.
    void play_move(Position);

#ifdef CS211_TESTING
    // When this class is compiled for testing, members of a struct named
    // Test_access will be allowed to access private members of this class.
    friend struct Test_access;
#endif

private:
    Player turn_   = Player::black;
    Player winner_ = Player::neither;
    Board board_;

public:
    int scores[2];

private:
    Move_map next_moves_;

    Move_map mm;

    // ========================
    // PRIVATE HELPER FUNCTIONS
    // ========================

    //takes in a player and returns true if said player is not the player whose
    // turn it is; false otherwise
    bool is_opp(Player p) const;

    // takes a position and returns position set of  all possible moves
    // for each position, adds any jumped positions to mm as a position set
    Position_set check_jumps_(Position current);

    // changes the turn to the next player, or sets the game over if one player
    // collects all their opponents pieces
    void advance_turn_();

    // ends the game by setting player to neither and declaring a winner
    void set_game_over_();

    // takes a played move and set the relevant board positions (removes any
    // jumped pieces and starting piece from start posn)
    void really_play_move_(Position played);
};
