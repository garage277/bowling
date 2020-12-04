#include <iostream>
#include <gtest/gtest.h>
#include "../utils.h"
#include "../frame.h"
#include "../game.h"
#include "../multiplayer_game.h"

using namespace bowling;

TEST(CycleCounterTest, test1){
    CycleCounter cc{2};

    ASSERT_EQ(cc.next(),0);
    ASSERT_EQ(cc.next(),1);
    ASSERT_EQ(cc.next(),0);
}

TEST(FrameTest, test1){
    Frame fr;
    fr.add_ball(10);
    fr.add_bonus(10);
    fr.add_bonus(10);
    ASSERT_EQ(fr.get_total(), 30);
}

TEST(FrameTest, test2){
    Frame fr;
    fr.add_ball(2);
    fr.add_ball(8);
    ASSERT_EQ(fr.get_state(), Frame::State::Spare);
    ASSERT_EQ(fr.get_bonus_counter(), 1);
    fr.add_bonus(10);
    ASSERT_EQ(fr.get_total(), 20);
}

TEST(GameTest, all_strikes){
    Game g;
    for(uint8_t i=0; i<12; ++i)
        g.add_ball(10);
    ASSERT_EQ(g.get_state(), XRC::Done);
    ASSERT_EQ(g.get_score(), 300);
}

TEST(GameTest, all_strikes_until_done){
    Game g;
    for(auto rc=XRC::More; rc!=XRC::Done; )
        rc = std::get<0>(g.add_ball(10));
    ASSERT_EQ(g.get_score(), 300);
}

TEST(GameTest, no_strikes){
    Game g;
    for(auto rc=XRC::More; rc!=XRC::Done; )
        rc = std::get<0>(g.add_ball(1));
    ASSERT_EQ(g.get_score(), 20);
}

TEST(GameTest, mix){
    Game g;
    std::vector<uint8_t> v{
        1,2,    //frame 1, score 3                      =3
        10,     //frame 2, score 10 --Strike--> +5 +5   =23
        5,5,    //frame 3, score 10 --Spare---> +4      =37
        4,2,    //frame 4, score 6                      =43
        1,0,    //frame 5, score 1                      =44
        1,0,    //frame 6, score 1                      =45
        1,0,    //frame 7, score 1                      =46
        1,0,    //frame 8, score 1                      =47
        1,0,    //frame 9, score 1                      =48
        1,0     //frame 10, score 1                     =49
    };

    for(auto i : v)
        g.add_ball(i);

    ASSERT_EQ(g.get_state(), XRC::Done);
    ASSERT_EQ(g.get_score(), 49);
}

TEST(GameTest, mix2){
    Game g;
    std::vector<uint8_t> v{
        1,2,    //frame 1, score 3                      =3
        10,     //frame 2, score 10 --Strike--> +5 +5   =23
        5,5,    //frame 3, score 10 --Spare---> +4      =37
        4,2,    //frame 4, score 6                      =43
        1,0,    //frame 5, score 1                      =44
        1,0,    //frame 6, score 1                      =45
        1,0,    //frame 7, score 1                      =46
        1,0,    //frame 8, score 1                      =47
        1,0,    //frame 9, score 1                      =48
        10,1,1  //frame 10, score 10 --Strike extra--> +1 +1  =60
    };

    for(auto i : v)
        g.add_ball(i);

    ASSERT_EQ(g.get_state(), XRC::Done);
    ASSERT_EQ(g.get_score(), 60);
}

TEST(GameTest, mix3){
    Game g;
    std::vector<uint8_t> v{
        1,2,    //frame 1, score 3                      =3
        10,     //frame 2, score 10 --Strike--> +5 +5   =23
        5,5,    //frame 3, score 10 --Spare---> +4      =37
        4,2,    //frame 4, score 6                      =43
        1,0,    //frame 5, score 1                      =44
        1,0,    //frame 6, score 1                      =45
        1,0,    //frame 7, score 1                      =46
        1,0,    //frame 8, score 1                      =47
        1,0,    //frame 9, score 1                      =48
        10,10,10  //frame 10, score 10 --Strike extra--> +10 +10  =78
    };

    for(auto i : v)
        g.add_ball(i);

    ASSERT_EQ(g.get_state(), XRC::Done);
    ASSERT_EQ(g.get_score(), 78);
}

TEST(GameTest, ex1){
    Game g;
    ASSERT_THROW(g.add_ball(11), std::invalid_argument);
}

TEST(MultiplayerGameTest, one_player){
    MultiplayerGame mg({"Ivan"});
    while( mg.get_state() != XRC::Done )
        mg.add_ball(1);

    ASSERT_EQ(mg.get_score("Ivan"), 20);
}

TEST(MultiplayerGameTest, two_players){
    MultiplayerGame mg({"Ivan", "Petr"});
    while( mg.get_state() != XRC::Done )
        mg.add_ball(1);

    ASSERT_EQ(mg.get_score("Ivan"), 20);
    ASSERT_EQ(mg.get_score("Petr"), 20);
}

TEST(MultiplayerGameTest, two_players2){
    MultiplayerGame mg({"Ivan", "Petr"});
    while( mg.get_state() != XRC::Done ){
        if( mg.who_is_next() == "Ivan" )
            mg.add_ball(1);
        if( mg.who_is_next() == "Petr" )
            mg.add_ball(10);
    }

    ASSERT_EQ(mg.get_score("Ivan"), 20);
    ASSERT_EQ(mg.get_score("Petr"), 300);
}


int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
