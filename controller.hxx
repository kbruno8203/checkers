#pragma once

#include "model.hxx"
#include "view.hxx"

#include <ge211.hxx>

class Controller : public ge211::Abstract_game
{
public:
    explicit Controller(int size = 8);
    Controller(int width, int height);

protected:
    void on_mouse_move(ge211::Posn<int>) override;
    void on_mouse_down(ge211::events::Mouse_button, ge211::Posn<int>)
    override;
    void draw(ge211::Sprite_set& set) override;
    View::Dimensions initial_window_dimensions() const override;
    std::string initial_window_title() const override;

private:
    Model model_;
    View view_;

    ge211::Posn<int> mouse_posn = {0,0};
};

