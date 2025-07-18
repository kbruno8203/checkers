#pragma once

#include <iostream>

enum class Player
{
    black,
    red,
    black_king,
    red_king,
    neither,
};


// Returns the other player
Player other_player(Player);

// Prints a player
std::ostream& operator<<(std::ostream&, Player);
