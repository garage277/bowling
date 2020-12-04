#ifndef __BOWLING_UTILS_H_
#define __BOWLING_UTILS_H_
#include <stdexcept>

namespace bowling{

const uint8_t gcStrikeN = 10;
const uint8_t gcStrikeBonusesN = 2;
const uint8_t gcSpareBonusesN = 1;
const uint8_t gcFrameN = 10;
const uint8_t gcMaxPlayers = 5;
const uint8_t gcExtraThrowsStrike = 2;
const uint8_t gcExtraThrowsSpare = 1;

enum class XRC : bool{
    More = true, //надо продолжить работу с объектом
    Done = false //нельзя продолжать работу с объектом
};

//бегает от 0 до (N-1)
class CycleCounter{
public:
    CycleCounter(size_t n) : N(n){
        if( n == 0 )
            throw std::invalid_argument("0 нельзя");
    }

    size_t next(){
        if( counter == N )
            counter = 0;
        return counter++;
    }

private:
    size_t counter{0};
    size_t N{0}; //
};

} //namespace bowling
#endif
