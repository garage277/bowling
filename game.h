#ifndef __BOWLING_GAME_H_
#define __BOWLING_GAME_H_

#include <array>
#include <list>
#include <tuple>

#include "frame.h"

namespace bowling{

//класс описывает игру из десяти фреймов
class Game{
    std::array<Frame, gcFrameN> frames; //в игре gcFrameN сетов
    std::list<size_t> waits_for_bonus; //какие фреймы сейчас ждут бонуса
    uint8_t current_frame_n{0}; //текущий фрейм
    XRC state{XRC::More}; //состояние игры, завершена или нет

    //страйк в 10 фрейме
    bool extracondition{false};
    uint8_t extraThrows{0}; //если страйк в последнем фрейме, то апнится до gcExtraThrows

public:
    //первый - состояние игры: Done - игра завершена, More - игра продолжается
    //второй - состояние игрока: Done - текущая серия бросков завершена, More - не завершена
    std::tuple<XRC,XRC> add_ball(uint8_t points){
        if( state == XRC::Done )
            std::logic_error("the game is already over");

        auto &x = frames.at(current_frame_n); //out_of_range?

        //начисляем бонусы ожидающим
        //броски по экстраусловиям тоже идут в бонусы
        for(auto i=waits_for_bonus.begin(); i!=waits_for_bonus.end();){
            auto &tmp_frame = frames.at(*i);
            tmp_frame.add_bonus(points);
            if( tmp_frame.get_bonus_counter() == 0 )
                i = waits_for_bonus.erase(i);
            else
                ++i;
        }

        //экстраусловия обрабатываем перед add_ball,
        //т.к. последний фрейм уже закрыт
        if( extracondition && extraThrows > 0){
            x.add_bonus(points);
            state = --extraThrows == 0 ? XRC::Done : XRC::More;
            return std::make_tuple(state,state); //игра завершится, когда завершатся экстраброски
        }

        auto rc = x.add_ball(points);

        if( rc == XRC::More )
            return std::make_tuple(state,XRC::More); //ни игра, ни серия не завершены

        //фрейм завершен

        if( current_frame_n == gcFrameN-1 ){
            //это последний фрейм
            //если страйк, то поднимаем 2 экстрабонуса
            if( x.get_state() == Frame::State::Strike ){
                extracondition = true;
                extraThrows = gcExtraThrowsStrike;
                return std::make_tuple(state,XRC::More); //фрейм завершен, но есть экстраброски. и игра и серия продолжаются
            }

            //если страйк, то поднимаем 1 экстрабонус
            if( x.get_state() == Frame::State::Spare ){
                extracondition = true;
                extraThrows = gcExtraThrowsSpare;
                return std::make_tuple(state,XRC::More); //фрейм завершен, но есть экстраброски. и игра и серия продолжаются
            }

            //не страйк. всё, игра завершена
            state = XRC::Done;
            return std::make_tuple(state, XRC::Done); //и игра и серия завершены
        }

        //это не последний фрейм
        if( x.get_state() == Frame::State::Strike || x.get_state() == Frame::State::Spare )
            waits_for_bonus.push_back(current_frame_n);

        ++current_frame_n;
        return std::make_tuple(state, XRC::Done); //игра не завершена, но серия завершена, надо дать кинуть другим
    }

    XRC get_state(){return state;}
    uint32_t get_frame_number(){return current_frame_n+1;} //для вывода пользователю
    size_t get_score(){
        size_t score{0};
        for( auto &fr : frames )    // std:: алгоритм?
            score += fr.get_total();
        return score;
    }
};

}//namespace bowling

#endif
