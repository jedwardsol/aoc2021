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

using namespace std::literals;
#include "include/thrower.h"

extern std::istringstream data;



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

    std::ranges::sort(crabs);

    auto median = (  crabs[crabs.size()/2]
                   + crabs[crabs.size()/2-1]) / 2;
/*

    double sum = std::accumulate(crabs.begin(), crabs.end(),0);

    auto  mean = sum/crabs.size();

    int  bestPos = std::round(mean);

*/
    int energy{};

    for(auto crab : crabs)
    {
        energy+= std::abs(crab-median);
    
    }

    std::cout << ' ' << energy << '\n';

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}