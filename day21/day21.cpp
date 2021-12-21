#define NOMINMAX
#include <cassert>
#include <cstdint>

#include <map>
#include <array>


#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"


struct DeterministicD100 
{
    int roll() 
    {
        timesRolled++;
        faceShowing++;
        faceShowing%=100;;

        return faceShowing+1;
    }

    int     faceShowing{99};    
    int     timesRolled{};
};

struct Player
{
    int16_t position;
    int16_t score{};

    auto operator<=>(Player const &rhs) const noexcept = default;
};

struct Game
{
    Player player[2];

    auto operator<=>(Game const &rhs) const noexcept = default;
};

constexpr Game    initialState{{{4-1,0},{5-1,0}}};      // my input data

using Multiverse = std::map<Game, int64_t>;             // state -> count of universes in this state



void part1()
{
    DeterministicD100   die;
    Game                game{initialState};
    int                 currentPlayer{0};

    while(   game.player[0].score <= 1000
          && game.player[1].score <= 1000)
    {
        game.player[currentPlayer].position += die.roll() + die.roll() + die.roll();
        game.player[currentPlayer].position %= 10;
        game.player[currentPlayer].score    += game.player[currentPlayer].position+1;

        currentPlayer++;
        currentPlayer %= 2;
    }

    std::cout << std::format("part 1 : {}\n",std::min(game.player[0].score,game.player[1].score) * die.timesRolled);;
}

auto generateOutcomes()
{
    std::map<int,int>           outcomes{};         // score -> number of times that score occurs

    for(int die1 : {1,2,3})
    {
        for(int die2 : {1,2,3})
        {
            for(int die3 : {1,2,3})
            {
                outcomes[die1+die2+die3]++;
            }
        }
    }

    return outcomes;
}

Multiverse  updateMultiverse(Multiverse const &previousMultiverse,std::array<int64_t,2> &wins,int currentPlayer)
{
    static auto const      dieOutcomes{generateOutcomes()};       

    Multiverse multiverse;


    for(auto [previousGameState,stateCount] : previousMultiverse)
    {
        for(auto [roll, rollCount] : dieOutcomes)
        {
            Game  gameState{previousGameState};

            gameState.player[currentPlayer].position += roll;
            gameState.player[currentPlayer].position %= 10;
            gameState.player[currentPlayer].score    += gameState.player[currentPlayer].position+1;

            if(gameState.player[currentPlayer].score >= 21)
            {
                wins[currentPlayer]   += stateCount * rollCount;
            }
            else
            {
                multiverse[gameState] += stateCount * rollCount; 
            }
        }
    }

    return multiverse;
}


void part2()
{
    std::array<int64_t,2>       wins{};
    Multiverse                  multiverse;          
    int                         currentPlayer{};

    multiverse[initialState]=1;                      // start with 1 universe


    while(!multiverse.empty())
    {
        multiverse = updateMultiverse(multiverse,wins,currentPlayer);

        currentPlayer++;
        currentPlayer %= 2;
    }

    std::cout << std::format("part 2 : {}\n",std::max(wins[0],wins[1]));
}

int main()
try
{
    part1();
    part2();

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}