#include "view.hxx"

//type aliases
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

//Colors
static ge211::Color const red  = Color::medium_red();
static ge211::Color const white = Color::white();
static ge211::Color const gold = Color::medium_yellow();
static ge211::Color const black = Color::black();
static ge211::Color const grey = {128, 128, 128};

static int const grid_size = 70;

View::View(Model const& model)
        : model_(model),

        board_grey_tile({grid_size - 1,grid_size -1}, grey),
        board_white_tile({grid_size - 1,grid_size -1}, white),
        board_gold_tile({grid_size - 1,grid_size -1}, gold),
        red_mouse(grid_size/8, red),
        black_mouse(grid_size/8, black),
        red_piece_sprite(grid_size/2 - (grid_size/8), red),
        black_piece_sprite(grid_size/2 - (grid_size/8), black),
        grey_square_sprite({grid_size - 1,grid_size -1},grey),
        red_king_sprite("red_king.png"),
        black_king_sprite("black_king.png")
{ }

void
View::draw(ge211::Sprite_set& set, ge211::Posn<int> position)
{
    add_mouse_(set,model_.turn(),position,5);

    // highlight selected piece
    if(model_.sel_posn != ge211::the_origin) {
        set.add_sprite(board_gold_tile,
                       board_to_screen(model_.sel_posn), 1);
    }

    //highlight available moves
    for (auto p: model_.poss_moves){
        set.add_sprite(board_gold_tile,
                       board_to_screen(p), 1);
    }

    // ====================
    // set up board display
    // ====================
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            if ((row % 2 == 0 && col % 2 == 0) || (row % 2 != 0 && col % 2 !=
            0)){
                set.add_sprite(board_white_tile,
                               board_to_screen({row,col}),
                               0);
            }
            else if((row % 2 == 0 && col != 0) || (row % 2 != 0 && col % 2 ==
            0)){
                set.add_sprite(board_grey_tile,
                               board_to_screen({row,col}),
                               0);
            }
        }
    }

    // ==============
    // Display Scores
    // ==============
    int red_score = model_.scores[0];
    int black_score = model_.scores[1];

    ge211::Text_sprite::Builder red_current_score(sans18);
    red_current_score.color(Color::black());
    red_current_score << "Red Score: ";
    red_current_score.add_message(red_score);

    red_score_label.reconfigure(red_current_score);
    set.add_sprite(red_score_label, Position{1, 1}, 5);

    ge211::Text_sprite::Builder black_current_score(sans18);
    black_current_score.color(Color::black());
    black_current_score << "Black Score: ";
    black_current_score.add_message(black_score);

    black_score_label.reconfigure(black_current_score);
    set.add_sprite(black_score_label,
                   Position{6 * grid_size,1},
                   5);

    // =====================
    // Pieces on board
    // =====================
    for (auto pos: model_.board()){
        if (model_.operator[](pos) == Player::black) {
            add_player_sprite_(set, Player::black, pos, 4);
        }

        else if (model_.operator[](pos) == Player::red) {
            add_player_sprite_(set, Player::red, pos, 4);
        }

        else if (model_.operator[](pos) == Player::red_king){
            add_player_sprite_(set, Player::red_king, pos, 4);
        }

        else if (model_.operator[](pos) == Player::black_king){
            add_player_sprite_(set, Player::black_king, pos, 4);
        }
        else if (model_.operator[](pos) == Player::neither){
        }

        if(model_.winner() == Player::red) {
            ge211::Text_sprite::Builder red_wins(sans18);
            red_wins.color(Color::black());
            red_wins << "Game Over: Red is the winner!";
            red_wins_output.reconfigure(red_wins);
            set.add_sprite(red_wins_output,
                           Position {2 * grid_size, 2 * grid_size},
                           5);
        }

        if(model_.winner() == Player::black){
            ge211::Text_sprite::Builder black_wins(sans18);
            black_wins.color(Color::black());
            black_wins << "Game Over: Black is the winner!";
            black_wins_output.reconfigure(black_wins);
            set.add_sprite(black_wins_output,
                           Position {2 * grid_size, 2 * grid_size},
                           5);
        }
    }
}

View::Dimensions
View::initial_window_dimensions() const
{
    return grid_size * Dimensions{8, 8};
}

std::string
View::initial_window_title() const
{
    return "Checkers";
}

View::Position
View::board_to_screen(Model::Position pos)
{
    return {grid_size * pos.x, grid_size * pos.y};
}

Model::Position
View::screen_to_board(View::Position pos)
{
    return {pos.x / grid_size, pos.y / grid_size};
}

Model::Position
View::center_circle(Model::Position pos) const
{
    pos.x += grid_size/8;
    pos.y += grid_size/8;
    return pos;
}

void
View::add_player_sprite_(
        Sprite_set& set,
        Player player,
        Position pos,
        int z) const
{
    if (player == Player::red){
        set.add_sprite(red_piece_sprite,
                       center_circle(board_to_screen(pos)),z);
    }
    else if (player == Player::black){
        set.add_sprite(black_piece_sprite,
                       center_circle(board_to_screen(pos)),z);
    }

    else if (player == Player::red_king){
        set.add_sprite(red_king_sprite,{board_to_screen(pos).x +
        grid_size/6,board_to_screen(pos).y + grid_size/5}, z,
                       ge211::geometry::Transform::scale (0.05));
    }

    else if (player == Player::black_king){
        set.add_sprite(black_king_sprite,{board_to_screen(pos).x +
        grid_size/6,board_to_screen(pos).y + grid_size/5}, z,
                       ge211::geometry::Transform::scale (0.085));
    }
}

void
View::add_mouse_(
        Sprite_set& set,
        Player player,
        Position pos,
        int z) const
{
    if (player == Player::red){
        set.add_sprite(red_mouse, pos,z);
    }
    else if (player == Player::black){
        set.add_sprite(black_mouse,pos,z);
    }
}

