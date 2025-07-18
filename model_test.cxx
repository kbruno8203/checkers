#include "model.hxx"
#include <catch.hxx>

using namespace ge211;

struct Test_access
{
    Model& model;

    explicit Test_access(Model&);

    // Sets the player at posn to player
    void set_player(Model::Position posn, Player player);

    // access sel_posn and poss_moves
    Position_set& poss_moves();
    Model::Position& sel_posn();
    Player&
    turn_();
};

TEST_CASE("Check Directions")
{
    Model model(8, 8);
    Test_access a(model);

    a.turn_() = Player::black;

    for (auto posn : model.board()) {
        a.set_player({posn},Player::neither);
    }

    a.set_player({1,6}, Player::black);
    model.play_move({1,6});
    CHECK(model.sel_posn.x == 1);
    CHECK(model.sel_posn.y == 6);

    // check player can move forward
    CHECK(model.poss_moves[{0,5}]);
    CHECK(model.poss_moves[{2,5}]);

    // check player cannot move backward
    CHECK_FALSE(model.poss_moves[{0,7}]);
    CHECK_FALSE(model.poss_moves[{2,7}]);

    model.play_move({0,5});

    CHECK(a.turn_() == Player::red);
    a.set_player({2,1}, Player::red);

    model.play_move({2,1});
    CHECK(model.sel_posn.x == 2);
    CHECK(model.sel_posn.y == 1);

    // check player can move forward
    CHECK(model.poss_moves[{1,2}]);
    CHECK(model.poss_moves[{3,2}]);

    // check player cannot move backward
    CHECK_FALSE(model.poss_moves[{1,0}]);
    CHECK_FALSE(model.poss_moves[{3,0}]);

    model.play_move({1,2});
    CHECK(a.turn_() == Player::black);

    a.set_player({5,6}, Player::black_king);
    CHECK(model.operator[]({5,6})==Player::black_king);

    model.play_move({5,6});
    CHECK(model.sel_posn.x == 5);
    CHECK(model.sel_posn.y == 6);

    // check player can move in all directions
    CHECK(model.poss_moves[{4,5}]);
    CHECK(model.poss_moves[{4,5}]);
    CHECK(model.poss_moves[{4,7}]);
    CHECK(model.poss_moves[{6,7}]);

    model.play_move({6,7});

    CHECK(a.turn_() == Player::red);
    a.set_player({6,2}, Player::red_king);

    model.play_move({6,2});
    CHECK(model.sel_posn.x == 6);
    CHECK(model.sel_posn.y == 2);

    // check player can move in all directions
    CHECK(model.poss_moves[{5,1}]);
    CHECK(model.poss_moves[{7,1}]);
    CHECK(model.poss_moves[{5,3}]);
    CHECK(model.poss_moves[{7,3}]);
}


TEST_CASE("Check Invalid Moves")
{
    Model model(8, 8);
    Test_access a(model);

    a.turn_() = Player::black;

    //clear the board
    for (auto posn : model.board()) {
        a.set_player({posn},Player::neither);
    }

    //set 4 players
    a.set_player({7, 0}, Player::red);
    CHECK(model.operator[]({7, 0}) == Player::red);
    a.set_player({0, 7}, Player::black);
    CHECK(model.operator[]({0, 7}) == Player::black);
    a.set_player({6, 1}, Player::red);
    CHECK(model.operator[]({6, 1}) == Player::red);
    a.set_player({1, 6}, Player::black);
    CHECK(model.operator[]({1, 6}) == Player::black);

    //attempt to move pieces onto white space
    model.play_move({0,7});
    model.play_move({0,6});
    model.play_move({7,0});
    model.play_move({7,1});
    CHECK(model.operator[]({0, 7}) == Player::black);
    CHECK(model.operator[]({7, 0}) == Player::red);

    //attempt to move piece onto space occupied by another piece
    model.play_move({0,7});
    model.play_move({1,6});
    model.play_move({7,0});
    model.play_move({6,1});
    CHECK(model.operator[]({0, 7}) == Player::black);
    CHECK(model.operator[]({7, 0}) == Player::red);

    //attempt to move backwards by a piece that's not a king
    model.play_move({1,6});
    model.play_move({2,7});
    model.play_move({6,1});
    model.play_move({5,0});
    CHECK(model.operator[]({1, 6}) == Player::black);
    CHECK(model.operator[]({6, 1}) == Player::red);
}


TEST_CASE("Check Single Jumps"){
    Model model(8, 8);
    Test_access a(model);

    a.turn_() = Player::black;

    //clear the board
    for (auto posn : model.board()) {
        a.set_player({posn},Player::neither);
    }

    //set players
    a.set_player({3, 4}, Player::red);
    CHECK(model.operator[]({3, 4}) == Player::red);
    a.set_player({2, 5}, Player::black);
    CHECK(model.operator[]({2, 5}) == Player::black);
    a.set_player({5, 2}, Player::red);
    CHECK(model.operator[]({5, 2}) == Player::red);

    //black jumps red on 3, 4
    model.play_move({2,5});
    model.play_move({4,3});
    CHECK(model.operator[]({3, 4}) == Player::neither);
    CHECK(model.operator[]({4, 3}) == Player::black);
    CHECK(model.scores[1] == 1);
    CHECK(model.scores[0] == 0);

    //red jumps black on 4,3
    model.play_move({5,2});
    model.play_move({3,4});
    CHECK(model.operator[]({3, 4}) == Player::red);
    CHECK(model.operator[]({4, 3}) == Player::neither);
    CHECK(model.scores[1] == 1);
    CHECK(model.scores[0] == 1);
}


TEST_CASE("Check King")
{
    Model model(8, 8);
    Test_access a(model);

    a.turn_() = Player::black;

    //clear the board
    for (auto posn : model.board()) {
        a.set_player({posn},Player::neither);
    }

    //set two players
    a.set_player({7, 0}, Player::red);
    CHECK(model.operator[]({7, 0}) == Player::red);
    a.set_player({0, 7}, Player::black);
    CHECK(model.operator[]({0, 7}) == Player::black);

    // allow possible move to end of board - black player
    a.poss_moves() = {{1,0}};
    a.sel_posn() = {0,7};

    // check no piece at original place and king at end space
    model.play_move({1,0});
    CHECK(model.operator[]({1,0}) == Player::black_king);
    CHECK(model.operator[]({0,7}) == Player::neither);
    CHECK(model.turn() == Player::red);

    // allow red player to move
    a.poss_moves() = {{6,7}};
    a.sel_posn() = {7,0};

    // check no piece at original place and king at end space
    model.play_move({6,7});
    CHECK(model.operator[]({6,7}) == Player::red_king);
    CHECK(model.operator[]({7,0}) == Player::neither);
    CHECK(model.turn() == Player::black);

    // sets selected position to 0,7 and populates poss_moves
    model.play_move({1,0});

    // check that black king can now move in all directions
    CHECK(model.poss_moves[{2,1}]);
    CHECK(model.poss_moves[{0, 1}]);
    model.play_move({2,1});

    // sets selected position to 1,0 and populates poss_moves
    model.play_move({6,7});

    // check that red king can now move in all directions
    CHECK(model.poss_moves[{7,6}]);
    CHECK(model.poss_moves[{5,6}]);
}


TEST_CASE("Check Double Jumps")
{
    Model model(8, 8);
    Test_access a(model);

    a.turn_() = Player::black;

    //clear the board
    for (auto posn : model.board()) {
        a.set_player({posn},Player::neither);
    }

    //set three players
    a.set_player({5, 2}, Player::red);
    CHECK(model.operator[]({5, 2}) == Player::red);
    a.set_player({3, 4}, Player::red);
    CHECK(model.operator[]({3, 4}) == Player::red);
    a.set_player({2, 5}, Player::black);
    CHECK(model.operator[]({2, 5}) == Player::black);

    // black player makes double jump
    model.play_move({2,5});
    model.play_move({6,1});

    CHECK(model.operator[]({6,1}) == Player::black);
    CHECK(model.operator[]({5,2}) == Player::neither);
    CHECK(model.operator[]({3,4}) == Player::neither);
    CHECK(model.operator[]({2,5}) == Player::neither);

    CHECK(model.scores[1] == 2);

    // set new pieces
    a.set_player({7, 0}, Player::red);
    CHECK(model.operator[]({7, 0}) == Player::red);
    a.set_player({4, 3}, Player::black);
    CHECK(model.operator[]({4, 3}) == Player::black);

    // red player makes double jump
    model.play_move({7,0});
    model.play_move({3,4});

    CHECK(model.operator[]({3,4}) == Player::red);
    CHECK(model.operator[]({6,1}) == Player::neither);
    CHECK(model.operator[]({4,3}) == Player::neither);
    CHECK(model.operator[]({7,0}) == Player::neither);

    CHECK(model.scores[0] == 2);
}


TEST_CASE("Full Game")
{
    Model model(8, 8);
    Test_access a(model);

    CHECK(model.turn() == Player::black);
    CHECK(model.winner() == Player::neither);

    //first move: black on 4, 5 --> 3, 4
    model.play_move({4,5});
    model.play_move({3,4});

    //leaves original tile empty
    CHECK(model.operator[]({4, 5}) == Player::neither);
    CHECK(model.operator[]({3, 4}) == Player::black);
    CHECK(model.turn() == Player::red);
    CHECK(model.winner() == Player::neither);

    //second move: red on 3, 2 --> 4, 3
    model.play_move({3,2});
    model.play_move({4, 3});
    CHECK(model.operator[]({3, 2}) == Player::neither);

    //black on 0, 5 --> 1, 4
    model.play_move({0,5});
    model.play_move({1, 4});

    //red on 4, 3 --> 5, 4
    model.play_move({4,3});
    model.play_move({5, 4});

    //black on 6, 5 --> 4, 3 (jumps red player on 5, 4)
    model.play_move({6,5});
    model.play_move({4, 3});

    //check that red player is removed and black player now has 1 point:
    CHECK(model.operator[]({5, 4}) == Player::neither);
    CHECK(model.scores[1] == 1);

    //continue playing
    model.play_move({1,2});
    model.play_move({2, 3});
    model.play_move({1,4});
    model.play_move({3, 2});
    model.play_move({4,1});
    model.play_move({4, 5});

    //check no winner and that black == red
    CHECK(model.winner() == Player::neither);
    CHECK(model.scores[0] == model.scores[1]);

    //continue playing
    model.play_move({5,6});
    model.play_move({3, 4});
    model.play_move({5,2});
    model.play_move({6, 3});
    model.play_move({7,6});
    model.play_move({6, 5});
    model.play_move({0,1});
    model.play_move({1, 2});
    model.play_move({6,5});
    model.play_move({7, 4});
    model.play_move({3,0});
    model.play_move({4, 1});

    //black on 7, 4 double jumps red on 6, 3 and 4, 1 and earns king status
    model.play_move({7,4});
    model.play_move({3, 0});
    CHECK(model.operator[]({3, 0}) == Player::black_king);

    //check black score > red score but still no winner
    CHECK(model.scores[0] < model.scores[1]);
    CHECK(model.winner() == Player::neither);

    //check board positions
    Position_set blacks {{3, 0}, {4, 3}, {3, 4},
                         {2, 5}};
    for (auto posn : blacks) {
        CHECK((model[posn] == Player::black || model[posn] ==
        Player::black_king));
    }

    //continue playing
    model.play_move({7,2});
    model.play_move({6, 3});
    model.play_move({3,4});
    model.play_move({2, 3});
    model.play_move({6,3});
    model.play_move({7, 4});
    model.play_move({2,3});
    model.play_move({0, 1});
    model.play_move({6,1});
    model.play_move({7, 2});
    model.play_move({2,5});
    model.play_move({1, 4});
    model.play_move({7,4});
    model.play_move({6, 5});
    model.play_move({1,4});
    model.play_move({0, 3});
    model.play_move({7,2});
    model.play_move({6, 3});
    model.play_move({1,6});
    model.play_move({0, 5});
    model.play_move({5,0});
    model.play_move({4, 1});
    model.play_move({3,0});
    model.play_move({7, 4});

    //check black score == 9 (only 3 red pieces left on board)
    CHECK(model.scores[1] == 8);

    //finish game
    model.play_move({2,1});
    model.play_move({3, 2});
    model.play_move({4,3});
    model.play_move({2, 1});
    model.play_move({7,0});
    model.play_move({6, 1});
    model.play_move({2,1});
    model.play_move({3, 0});
    model.play_move({1,0});
    model.play_move({2, 1});
    model.play_move({3,0});
    model.play_move({1, 2});
    model.play_move({6,1});
    model.play_move({7, 2});
    model.play_move({3,6});
    model.play_move({4, 5});
    model.play_move({7,2});
    model.play_move({6, 3});
    model.play_move({7,4});
    model.play_move({5, 2});
    model.play_move({6,5});
    model.play_move({5, 6});
    model.play_move({4,7});
    model.play_move({6, 5});

    //game over: check winner
    CHECK(model.scores[1] == 12);
    CHECK(model.scores[0] == 2);
    CHECK(model.is_game_over());
    CHECK(model.winner() == Player::black);
}

// =============================
// Test Access Member Functions
// =============================
Test_access::Test_access(Model& model)
        : model(model)
{ }

void
Test_access::set_player(Model::Position posn, Player player)
{
    model.board_[posn] = player;
}

Player&
Test_access::turn_(){
    return model.turn_;
}

Position_set&
Test_access::poss_moves()
{
    return model.poss_moves;
}

Model::Position&
Test_access::sel_posn()
{
    return model.sel_posn;
}
