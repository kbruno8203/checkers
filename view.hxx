#pragma once

#include "model.hxx"

#include <string>

class View
{
public:
    using Dimensions = ge211::Dims<int>;
    using Position = ge211::Posn<int>;
    using Rectangle = ge211::Rect<int>;

    explicit View(Model const& model);

    void draw(ge211::Sprite_set& set, Position);

    Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    //converts board position to screen position
    static Position
    board_to_screen(Model::Position board_pos);

    //converts screen position to board position
    static Model::Position
    screen_to_board(Position screen_pos);

    Model::Position
    center_circle(Position pos) const;

private:
    Model const& model_;

    ge211::Font sans18{"sans.ttf", 20};
    ge211::Text_sprite red_score_label{"Red Score: ", sans18},
    black_score_label{"Black Score: ", sans18},
    red_wins_output{},
    black_wins_output{};

    void
    add_player_sprite_(ge211::Sprite_set&, Player, Position, int z) const;

    void
    add_mouse_(ge211::Sprite_set&, Player, Position, int z) const;

    ge211::sprites::Rectangle_sprite board_grey_tile;
    ge211::sprites::Rectangle_sprite board_white_tile;
    ge211::sprites::Rectangle_sprite board_gold_tile;
    ge211::sprites::Circle_sprite red_mouse;
    ge211::sprites::Circle_sprite black_mouse;
    ge211::sprites::Circle_sprite red_piece_sprite;
    ge211::sprites::Circle_sprite black_piece_sprite;
    ge211::sprites::Rectangle_sprite grey_square_sprite;
    ge211::sprites::Image_sprite red_king_sprite;
    ge211::sprites::Image_sprite black_king_sprite;
};

