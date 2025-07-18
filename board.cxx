#include "board.hxx"
#include <algorithm>

using namespace ge211;

Board::Board(Dimensions dims)
        : dims_(dims)
{
    if (dims_.width < 2 || dims_.height < 2) {
        throw Client_logic_error("Board::Board: dims too small");
    }

    if (dims_.width > Position_set::coord_limit ||
        dims_.height > Position_set::coord_limit) {
        throw Client_logic_error("Board::Board: dims too large");
    }
}

Board::Dimensions
Board::dimensions() const
{
    return dims_;
}

bool
Board::good_position(Position pos) const
{
    return 0 <= pos.x && pos.x < dims_.width &&
           0 <= pos.y && pos.y < dims_.height;
}

Player
Board::operator[](Position pos) const
{
    bounds_check_(pos);
    return get_(pos);
}

Board::reference
Board::operator[](Position pos)
{
    bounds_check_(pos);
    return reference(*this, pos);
}

//king moves both ways
std::vector<Board::Dimensions> const&
Board::king_directions(){
    static std::vector<Dimensions> result;
    result.push_back({-1,-1});
    result.push_back({-1,1});
    result.push_back({1,-1});
    result.push_back({1,1});
    return result;
}

// red moves down the board
std::vector<Board::Dimensions> const&
Board::red_directions(){
    static std::vector<Dimensions> result;
    result.push_back({-1,1});
    result.push_back({1,1});
    return result;
}

// black moves up the board
std::vector<Board::Dimensions> const&
Board::black_directions(){
    static std::vector<Dimensions> result;
    result.push_back({-1,-1});
    result.push_back({1,-1});
    return result;
}

Board::Rectangle
Board::all_positions() const
{
    return Rectangle::from_top_left(the_origin, dims_);
}

bool
operator==(Board const& b1, Board const& b2)
{
    return b1.dims_ == b2.dims_ &&
           b1.red_ == b2.red_ &&
           b1.black_ == b2.black_;
}

//returns player at position on board
Player
Board::get_(Position pos) const
{
    if (black_[pos]) {
        return Player::black;
    } else if (red_[pos]) {
        return Player::red;
    }
    else if (red_kings[pos]) {
        return Player::red_king;
    }
    else if (black_kings[pos]) {
        return Player::black_king;
    }
    else {
        return Player::neither;
    }
}


void
Board::set_(Position pos, Player player)
{
    switch (player) {
    case Player::black:
        black_[pos] = true;
        red_[pos] = false;
        red_kings[pos] = false;
        black_kings[pos] = false;
        break;

    case Player::red:
        black_[pos] = false;
        red_[pos] = true;
        red_kings[pos] = false;
        black_kings[pos] = false;
        break;

    case Player::red_king:
        black_[pos] = false;
        red_[pos] = false;
        red_kings[pos] = true;
        black_kings[pos] = false;
        break;

    case Player::black_king:
        black_[pos] = false;
        red_[pos] = false;
        red_kings[pos] = false;
        black_kings[pos] = true;
        break;

    default:
        black_[pos] = false;
        red_[pos] = false;
        red_kings[pos] = false;
        black_kings[pos] = false;
    }
}


void
Board::set_all(Position_set pos_set, Player player)
{
    switch (player) {
    case Player::red:
        red_ |= pos_set;
        black_ &= ~pos_set;
        black_kings &= ~pos_set;
        red_kings &= ~pos_set;
        break;

    case Player::black:
        black_ |= pos_set;
        red_ &= ~pos_set;
        black_kings &= ~pos_set;
        red_kings &= ~pos_set;
        break;

    case Player::red_king:
        red_kings |= pos_set;
        black_ &= ~pos_set;
        red_ &= ~pos_set;
        black_kings &= ~pos_set;
        break;

    case Player::black_king:
        black_kings |= pos_set;
        black_ &= ~pos_set;
        red_ &= ~pos_set;
        red_kings &= ~pos_set;
        break;

    default:
        black_ &= ~pos_set;
        red_ &= ~pos_set;
        red_kings &= ~pos_set;
        black_kings &= ~pos_set;
    }
}

void
Board::bounds_check_(Position pos) const
{
    if (!good_position(pos)) {
        throw Client_logic_error("Board: position out of bounds");
    }
}

bool
operator!=(Board const& b1, Board const& b2)
{
    return !(b1 == b2);
}

std::ostream&
operator<<(std::ostream& os, Board const& board)
{
    Board::Dimensions dims = board.dimensions();

    for (int y = 0; y < dims.height; ++y) {
        for (int x = 0; x < dims.width; ++x) {
            os << board[{x, y}];
        }
        os << "\n";
    }

    return os;
}

Board::reference::reference(Board& board, Position pos) noexcept
        : board_(board),
          pos_(pos)
{ }

Board::reference&
Board::reference::operator=(reference const& that) noexcept
{
    *this = Player(that);
    return *this;
}

Board::reference&
Board::reference::operator =(Player player) noexcept
{
    board_.set_(pos_, player);
    return *this;
}

Board::reference::operator Player() const noexcept
{
    return board_.get_(pos_);
}

