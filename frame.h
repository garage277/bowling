#ifndef __BOWLING_FRAME_H_
#define __BOWLING_FRAME_H_

#include <cstdint>
#include <stdexcept>

#include "utils.h"


namespace bowling{

//класс описывает один из десяти фреймов игры
class Frame{
public:
    enum class State : uint8_t{
        Empty,		//не было бросков
        Strike,		//страйк
        First_done,	//это первый бросок и это не страйк
        Second_done, //совершено два броска, остались кегли
        Spare       //со второго броска выбили все кегли
    };

private:
    State state{State::Empty};
    uint8_t score{0};
    uint8_t bonus{0};
    uint8_t bonus_counter{0};

public:
    //More - в этом сете еще нужны броски, Done - можно переходить к следующему сету
    XRC add_ball(uint8_t points)
    {
        if( not (state == State::Empty || state == State::First_done) )
            throw std::logic_error("too many throws for this frame"); //лишний бросок

        if( state == State::Empty ){
            //первый бросок
            if( points == gcStrikeN ){
                //УРА, СТРАЙК
                state = State::Strike;
                score = points;
                bonus_counter = gcStrikeBonusesN; //верхний класс должен будет еще начислить 2 бонуса
                return XRC::Done;
            }else if( points < gcStrikeN ){
                state = State::First_done;
                score = points;
                return XRC::More;
            }else
                throw std::invalid_argument("too many points for one throw"); //слишком много очков для одного броска
        }

        if( state == State::First_done ){
            if( score + points > gcStrikeN )
                throw std::invalid_argument("too many points for two throws"); //слишком много очков для двух бросков

            if( score + points == gcStrikeN ){
                state = State::Spare;
                bonus_counter = gcSpareBonusesN;
            }else{
                state = State::Second_done;
            }

            score += points;

            return XRC::Done;
        }

        throw std::logic_error("WTF?!");
    }

    //More - нужны еще бонусы, Done - больше бонусов не надо
    XRC add_bonus(uint8_t points){
        if( not ( state == State::Strike || state == State::Spare ) )
            throw std::logic_error("why bonus?");

        if( bonus_counter == 0 )
            throw std::logic_error("too many bonuses");

        if( points > gcStrikeN )
            throw std::invalid_argument("too many points for bonus");

        --bonus_counter;
        bonus += points;

        return bonus_counter == 0 ? XRC::Done : XRC::More;
    }

    uint8_t get_score(){return score;}
    uint8_t get_bonus(){return bonus;}
    uint8_t get_total(){return score+bonus;}
    State get_state(){
        if( state == State::Empty )
            throw std::logic_error("запрос состояния пустого сета. скорее всего кто-то некорректно работает с XSet"); //избавим верхние классы от проверки лишнего состояния

        return state;
    }
    auto get_bonus_counter(){return bonus_counter;} //сколько еще бонусов надо

};

}//namespace bowling

#endif //__BOWLING_FRAME_H_
