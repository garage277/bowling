#ifndef __BOWLING_MULTIPLAYER_GAME_H_
#define __BOWLING_MULTIPLAYER_GAME_H_

#include <vector>
#include <string>
#include <cassert>
#include "game.h"

namespace bowling{

struct XGame{
    std::string player;
    Game game;
};

//класс описывает игру из десяти фреймов
class MultiplayerGame{
    XRC state{XRC::More}; //завершена игра или надо еще бросать
    std::vector<XGame> games; //
    size_t current_player{0}; //текущая игра. кто сейчас кидает
    size_t x_number{0}; //кол-во незавершенных игр, играем пока не обнулится
    CycleCounter cc{100500};
public:
    MultiplayerGame(const std::vector<std::string> &players){
        if( players.size() == 0 || players.size() > gcMaxPlayers )
            throw std::invalid_argument("invalid number of players");

        for( auto i=players.begin(); i!=players.end(); ++i )
            games.push_back({*i,Game()});

        x_number = players.size();
        cc = CycleCounter(players.size());
        current_player = cc.next(); //заводим CycleCounter. остались на нуле
        assert(current_player==0);
    }

    XRC get_state(){return state;}

    //кто должен кидать следующим
    std::string who_is_next(){
        if( state == XRC::Done )
            throw std::logic_error("the game is already over");

        return games.at(current_player).player;
    }

    //More - должен бросать who_is_next, Done - турнир закончен
    XRC add_ball(uint8_t points){
        if( state == XRC::Done )
            throw std::logic_error("the game is already over");

        if( games.at(current_player).game.get_state() == XRC::Done )
            throw std::logic_error("internal error...");

        XRC rc_game, rc_frame;
        std::tie(rc_game, rc_frame) = games.at(current_player).game.add_ball(points);
        if( rc_game == XRC::Done ){
            //игра для этого игрока закончена... идем по последнему кругу...
            --x_number;
            if( x_number == 0 ){
                //совсем всё
                state = XRC::Done;
                return state;
            }
            //должны закончить оставшиеся игроки...
            current_player = cc.next(); //переходим к следующему игроку
            return XRC::More;
        }

        //игра для текущего игрока пока продолжается, смотрим фрейм...

        if( rc_frame == XRC::More ){
            //у игрока еще есть ходы
            //не переключаемся на следующего, просто выходим
            return XRC::More;
        }

        //фрейм завершен
        current_player = cc.next();
        return XRC::More;
    }

    size_t get_score(const std::string &player){
        for( auto i=games.begin(); i!=games.end(); ++i )
            if( i->player == player )
                return i->game.get_score();
        throw std::invalid_argument("unknown player");
    }

    auto get_frame_number(){ return games.at(current_player).game.get_frame_number();}
};

}//namespace

#endif
