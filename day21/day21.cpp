#define NOMINMAX
#include <cassert>
#include <cstdint>

#include <random>

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
        nextRoll++;
        nextRoll%=100;;

        return nextRoll+1;
    }

    int     nextRoll{-1};    
    int     timesRolled{};
};

struct Player
{
    int position;
    int score{};
};

struct Game
{
    Player player[2];
};

void part1()
{
    DeterministicD100   die;
    Game                game{{{4-1,0},{5-1,0}}};
    int                 current{0};

    while(   game.player[0].score <= 1000
          && game.player[1].score <= 1000)
    {

        game.player[current].position += die.roll() + die.roll() + die.roll();
        game.player[current].position %= 10;
        game.player[current].score    += game.player[current].position+1;

        current++;
        current %= 2;
    }

    std::cout << "part 1 : "  << std::min(game.player[0].score,game.player[1].score) * die.timesRolled;
}

void part2()
{
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