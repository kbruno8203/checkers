#include "controller.hxx"

Controller::Controller(int size)
        : Controller(size, size)
{ }

Controller::Controller(int width, int height)
        : model_(width, height),
          view_(model_)
{ }

void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set, mouse_posn);
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

void
Controller::on_mouse_move(ge211::Posn<int> position)
{
    mouse_posn = position;
}

void
Controller::on_mouse_down(ge211::Mouse_button left, ge211::Posn<int> pos)
{
    model_.play_move(View::screen_to_board(pos));
}
