#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <deque>

#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"

extern std::istringstream data;


void part1(std::vector<int> const &rawFish)
{
    std::deque<int8_t>            fish(rawFish.begin(),rawFish.end());

    for(int day=0;day<80;day++)
    {
        for(auto &age : fish)
        {
            age--;

            if(age==-1)
            {
                age=6;
                fish.push_back(8);
            }
        }
    }

    std::cout << std::format("Part 1 : {}\n",fish.size());;
}


void part2(std::vector<int> const &fish)
{
    auto  today   = std::array<int64_t,9>{};   // today[x] = number of fish of age x

    for(auto age : fish)
    {
        today[age]++;
    }

    auto  numFish = [&]
    {
        return std::accumulate(std::begin(today),std::end(today),0LL);
    };


    assert(numFish()!=fish.size());

    for(int day=1;day<=256;day++)
    {
        std::ranges::rotate(today,std::begin(today)+1);
        
        today[6]+=today[8];

        if(   day==80
           || day==256)
        {
            std::cout << std::format("{:3} : {}\n",day,numFish());
        }
    }
}


int main()
try
{
    auto fish = std::vector<int>{};
    auto age  = 0;

    while(data >> age)
    {
        fish.push_back(age);
        data.ignore();
    }

    part1(fish);
    part2(fish);

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}