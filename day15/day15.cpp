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

extern std::istringstream unitData;
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

void print2(auto const &v)
{
    auto const size=v.size();
    auto const split=size/5;

    for(auto row =0; row<size;row++)
    {
        for(auto col =0; col<size;col++)
        {
            std::cout << std::format("{}",v[row][col]);

            if((col % split) == split-1)
            {
                std::cout << ' ';
            }
        }

        if((row % split) == split-1)
        {
            std::cout << '\n';
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

auto extend(Cave &littleCave)
{
    auto const  littleSize=static_cast<int>(littleCave.size());
    auto const  bigSize   =littleSize*5;

    auto        bigCave(std::vector(bigSize, std::vector<Risk>(bigSize,0)));


    for(auto row=0;row<bigSize;row++)
    {
        for(auto col=0;col<bigSize;col++)
        {
            auto manhattan = row/littleSize + col/littleSize;

            auto value     = littleCave[row % littleSize][col % littleSize]-1;
            value += manhattan;
            value = value % 9;
            value ++;

            bigCave[row][col]=value;
        }
    }


    return bigCave;
}


int main()
try
{
    auto unit=readCave(unitData);
    auto test=readCave(testData);
    auto real=readCave(realData);

    assert(downRightOnly(test) == 40);
    std::cout << "Part 1 : " << downRightOnly(real) << "\n";


    auto bigUnit=extend(unit);
    auto bigTest=extend(test);
    auto bigReal=extend(real);

    print2(bigUnit);


    assert(downRightOnly(bigTest) == 315);
    std::cout << "Part 2 : " << downRightOnly(bigReal) << "\n";

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}