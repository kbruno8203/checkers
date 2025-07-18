#include "model.hxx"

using namespace ge211;

Model::Model(int size)
    : Model(size, size)
{
}

Model::Model(int width, int height)
    : poss_moves({}),
      sel_posn(0,0),
      board_({width, height})
{
    //loop through a position set and create a new instance for each position
    Position_set red_starts = {{1,0}, {3,0}, {5,0},
                               {7,0},{0,1},{2,1},{4,1},
                             {6,1},{1,2},{3,2},{5,2},
                             {7,2}};

    for (auto pos: red_starts){
        board_.operator[](pos) = Player::red;
    }

    Position_set black_starts = {{0,5},{2,5},{4,5},
                                 {6,5},{1,6},{3,6},
                                 {5,6},
                               {7,6},{0,7},{2,7},{4,7},
                               {6,7}};

    for (auto pos: black_starts){
        board_.operator[](pos) = Player::black;
    }

    turn_ = Player::black;
    scores[0] = 0; //red score
    scores[1] = 0; //black score
    winner_ = Player::neither;
}

Model::Rectangle
Model::board() const
{
    return board_.all_positions();
}

Player
Model::operator[](Position pos) const
{
    return board_[pos];
}

//plays a move at the given position if allowed
void
Model::play_move(Position p)
{
     if(is_game_over()) {
     }

     else if (!is_opp(board_.operator[](p)) && board_.operator[](p)
     != Player::neither) {
         sel_posn = p;
         poss_moves = check_jumps_(sel_posn);
     }

     else if (!is_opp(board_.operator[](sel_posn)) && board_.operator[]
     (p) == Player::neither) {
         for (auto posn: poss_moves) {
             if (posn == p) {
                 poss_moves = {};
                 really_play_move_(p);
             }
         }
     }
}

//=================
// Helper Functions
//=================
bool
Model::is_opp(Player p) const
{
    if((turn_ == Player::red || turn_ == Player::red_king) && ((p ==
    Player::black) || (p == Player::black_king))){
        return true;
    }
    else if((turn_ == Player::black || turn_ == Player::red_king) && ((p ==
    Player::red) || (p == Player::red_king))){
        return true;
    }
    return false;
}


Position_set
Model::check_jumps_(Position current)
{
    Player curr_player = board_.operator[](current);
    std::vector<Board::Dimensions> dirs;
    Position_set playable_posns = Position_set();

    if (curr_player == Player::black_king || curr_player == Player::red_king){
        dirs = board_.king_directions();
    }
    else if (curr_player == Player::red){
        dirs = board_.red_directions();
    }
    else if (curr_player == Player::black){
        dirs = board_.black_directions();
    }

    // check adjacent positions and add them to a move map and a position set
    for (auto d : dirs){
        Position adj  = {current.x + d.width, current.y + d.height};
        if (board_.good_position(adj) && board_.operator[](adj) ==
        Player::neither){
            playable_posns |= {adj};
            mm[adj] = Position_set();
        }
    }

    for (auto d : dirs){
        // check if the adjacent player is the opposite
        Position p1 = {current.x + d.width, current.y + d.height};
        if (board_.good_position(p1) && is_opp(board_.operator[](p1))){

            // check if the next space is empty
            Position p2 = {p1.x + d.width, p1.y + d.height};
            if (board_.good_position(p2) && board_.operator[](p2) ==
            Player::neither){
                playable_posns |= {p2};
                mm[p2]=Position_set({p1});

                // check all double jumps for that spot
                for (auto d3: dirs){
                    Position p3 = {p2.x + d3.width, p2.y + d3.height};
                    if (board_.good_position(p3) && is_opp(board_.operator[]
                    (p3))){
                        Position p4 = {p3.x + d3.width, p3.y + d3.height};
                        if (board_.good_position(p4) && board_.operator[]
                        (p4) == Player::neither){
                            playable_posns |= {p4};
                            mm[p4] |= {p3};
                            mm[p4] |= mm[p2];
                        }
                    }
                }
            }
        }
    }
    return playable_posns;
}


void
Model::really_play_move_(Position played)
{
    next_moves_.clear();
    Position_set jumped = mm[played];
    Player moved = board_.operator[](sel_posn);

    // removed jumped pieces and original piece
    board_.set_all(jumped, Player::neither);
    board_.set_all({sel_posn}, Player::neither);

    // if hits the end, make it a king; set moved to Player
    if(turn_ == Player::red && played.y == 7){
        board_.set_all({played}, Player::red_king);
    }
    else if(turn_ == Player::black && played.y == 0){
        board_.set_all({played}, Player::black_king);
    }
    else {
        board_.set_all({played}, moved);
    }

    //update scores
    if (turn_ == Player::red || turn_ == Player::red_king){
        scores[0] = scores[0] + jumped.size();
    }
    else if (turn_ == Player::black || turn_ == Player::black_king){
        scores[1] = scores[1] + jumped.size();
    }

    sel_posn = {0,0};

    advance_turn_();
}


void
Model::advance_turn_()
{
    turn_ = other_player(turn_);

    for(int i = 0; i <=1; i++){
        if(scores[i] >= 12){
            set_game_over_();
        }
    }
}


void
Model::set_game_over_()
{
    turn_ = Player::neither;

    if (scores[0] > scores[1]){
        winner_ = Player::red;
    }
    else if (scores[1] > scores[0]){
        winner_ = Player::black;
    }
    else {
        winner_ = Player::neither;
    }
}




