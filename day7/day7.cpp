#define NOMINMAX
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
#include <functional>

using namespace std::literals;
#include "include/thrower.h"

extern std::istringstream data;

using EnergyCostFunc = std::function<int(int,int)>;


int totalCost(std::vector<int> const &crabs, int targetPosition, EnergyCostFunc  f)
{
    int energy{};

    for(auto crab : crabs)
    {
        energy+= f(crab,targetPosition);
    }

    return energy;
}


void bruteForce(std::vector<int> const &crabs, EnergyCostFunc f)
{
    auto [min,max] = std::ranges::minmax_element(crabs);

    auto minimumEnergy{ std::numeric_limits<int>::max()};


    for(auto candidate=*min; candidate<=*max; candidate++)
    {
        minimumEnergy = std::min(minimumEnergy, totalCost(crabs,candidate,f));
    }

    std::cout << minimumEnergy << '\n';
}


int main()
try
{
    auto crabs = std::vector<int>{};
    auto pos  = 0;

    while(data >> pos)
    {
        crabs.push_back(pos);
        data.ignore();
    }

    auto part1=[](int a,int b)
    {
        return std::abs(a-b);
    };

    auto part2=[](int a,int b)
    {
        auto distance = std::abs(a-b);

        return (distance*(distance+1))/2;
    };


    bruteForce(crabs,part1);
    bruteForce(crabs,part2);

    {
        std::ranges::sort(crabs);
        auto median = (  crabs[crabs.size()/2] + crabs[crabs.size()/2-1]) / 2;

        std::cout << totalCost(crabs,median,part1) << '\n';
    }

    {
        int mean1    = static_cast<int>(std::ceil ( std::accumulate(crabs.begin(), crabs.end(),0.0) / crabs.size() ));
        int mean2    = static_cast<int>(std::floor( std::accumulate(crabs.begin(), crabs.end(),0.0) / crabs.size() ));

        std::cout << std::min(totalCost(crabs,mean1,part2), totalCost(crabs,mean2,part2)) << '\n';
    }

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}