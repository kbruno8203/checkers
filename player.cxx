#include "player.hxx"

Player
other_player(Player player)
{
    switch (player) {
    case Player::red:
        return Player::black;
    case Player::black:
        return Player::red;
    default:
        return player;
    }
}

std::ostream&
operator<<(std::ostream& os, Player p)
{
    switch (p) {
    case Player::red:
        return os << "R";
    case Player::black:
        return os << "B";
    case Player::red_king:
        return os << "RK";
    case Player::black_king:
        return os << "BK";
    default:
        return os << "N";
    }
}

