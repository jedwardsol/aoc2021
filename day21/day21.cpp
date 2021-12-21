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

struct Die
{
    virtual int roll() = 0;
    int timesRolled() {return numTimesRolled;};


    int   numTimesRolled{};
};


struct DeterministicD100 : Die
{
    int roll() override
    {
        numTimesRolled++;

        nextRoll++;
        nextRoll%=100;;

        return nextRoll+1;
    }


    int nextRoll{-1};    
};


void part1()
{
    DeterministicD100   die;

    int player1Pos = 4 - 1;
    int player2Pos = 5 - 1;

    int player1Score{};
    int player2Score{};


    while(   player1Score <= 1000
          && player2Score <= 1000)
    {
        player1Pos += die.roll() + die.roll() + die.roll();
        player1Pos %= 10;
        player1Score += player1Pos+1;


        if(player1Score <= 1000)
        {
            player2Pos += die.roll() + die.roll() + die.roll();
            player2Pos %= 10;
            player2Score += player2Pos+1; 
        }
    }

    std::cout << "part 1 : "  << std::min(player1Score,player2Score) * die.timesRolled();


}


int main()
try
{
    part1();


    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}