#define NOMINMAX
#include <cassert>
#include <cstdint>

#include <array>
#include <vector>

#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"
#include "include/console.h"

extern std::istringstream testData;
extern std::istringstream realData;

using Risk=int;
using Cave= std::vector<std::vector<Risk>>;



void print(auto const &v)
{
    for(auto row : v)
    {
        for(auto i : row)
        {
            std::cout << std::format("{:3} ",i);
        }

        std::cout << "\n";
    }

    std::cout << "\n";
}

int downRightOnly(Cave  const &cave)
{
    auto const  size=static_cast<int>(cave.size());
    auto const  last=size-1;

    auto        total(std::vector(size, std::vector(size,0)));

    total[last][last]=cave[last][last];

    for(auto row=last-1; row>=0;row--)
    {
        total[row][last] = cave[row][last] + total[row+1][last];
    }

    for(auto col=last-1; col>=0;col--)
    {
        total[last][col] = cave[last][col] + total[last][col+1];
    }


    for(auto row=last-1; row>=0;row--)
    {
        for(auto col=last-1; col>=0;col--)
        {
            total[row][col] = cave[row][col] + std::min(total[row  ][col+1],
                                                        total[row+1][col  ]);
        }
    }

//  print(cave);
//  print(total);

    return total[0][0]-cave[0][0];
}


auto readCave(std::istringstream &data)
{
    Cave        cave;
    std::string line;

    while(std::getline(data,line))
    {
        if(!line.empty())
        {
            std::vector<Risk>   row;

            for(auto c: line)
            {
                assert(std::isdigit(c));
                row.push_back(c-'0');
            }

            cave.push_back(row);
        }
    }

    for(auto &row : cave)
    {
        assert(row.size()==cave.size());
        assert(row.size()==cave[0].size());
    }

    return cave;
}


int main()
try
{
    auto test=readCave(testData);
    auto real=readCave(realData);

    assert(downRightOnly(test) == 40);
    std::cout << "Part 1 : " << downRightOnly(real) << "\n";

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}