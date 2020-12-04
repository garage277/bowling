#include <iostream>
#include <stdexcept>
#include <vector>

#include "multiplayer_game.h"

using namespace std;
using namespace bowling;

int main(int, char**)
try{
    cout << endl << "ИГРАЕМ В БОУЛИНГ" << endl << endl;
    std::string s;
    std::vector<std::string> players;
    while( true ){
        s.clear();
        cout << "введите имя игрока или Enter, чтобы приступить к игре: " << endl;

        while(true){
            auto c = cin.get();
            if( c == '\n' )
                break;
            s.push_back(c);
        }

        if( s.empty() )
            break;

        players.push_back(s);
        cout << "добавлен игрок " << s << endl << endl;
    }

    cout << endl << "играем с игроками:" << endl;
    for(auto &s : players)
        cout << s << endl;

    MultiplayerGame mg(players);

    while( true ){
        if( mg.get_state() == XRC::Done )
            break;

        auto player = mg.who_is_next();
        cout << endl << "раунд #" << mg.get_frame_number() << " сейчас кидает " << player << endl
             << "введите количество сбитых кеглей: ";


        cin >> s;

        try{
            auto x = std::stoi(s);
            if( x<0 || x>10 )
                throw std::invalid_argument("wrong value");

            mg.add_ball(static_cast<uint8_t>(x));
        }catch(std::invalid_argument &ex){
            cout << "ошиблись при вводе? попробуйте еще раз" << endl;
            continue;
        }

    }

    cout << endl << "игра завершена. счет:" << endl;
    for( auto i : players )
        cout << i << " - " << mg.get_score(i) << endl;

    //TODO "if time runs out before the end of ten frames bowling stops,
    // and the person with the most points wins the game"
    //TODO 2. Store results to a file (optional)
    //TODO 3. Replay of the stored game (optional)

    return 0;
}catch(...){
    cerr << "программист поленился обработать все исключения?" << endl;
    return 1;
}
