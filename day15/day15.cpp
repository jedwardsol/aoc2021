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

#include "day15.h"





void printSmall   (Cave const &v)
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

void printExtended(Cave const &v)
{
    auto const size=v.size();
    auto const split=size/5 ? size/5 : 5;

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




auto readCave(std::istringstream &data)
{
    data.clear();
    data.seekg(0);

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

auto extend(Cave const &littleCave)
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
/*
    {
        auto const unitCave         =readCave(unitData);
        auto const extendedUnitCave =extend(unitCave);

        printExtended(unitCave);
        printExtended(extendedUnitCave);
    }
    {
        auto const hardCave=readCave(hardData);

        assert(downRightOnly(hardCave)          >  12);
        assert(Dijkstra(hardCave)              ==  12);
    }
*/


    {
        auto const testCave         =readCave(testData);
        auto const extendedTestCave =extend(testCave);

        assert(downRightOnly(testCave)         ==  40);
        assert(Dijkstra(testCave)              ==  40);

        assert(downRightOnly(extendedTestCave) == 315);
        assert(Dijkstra(extendedTestCave)      == 315);
    }



    auto const realCave         =readCave(realData);
    auto const extendedRealCave =extend(realCave);


    std::cout << "Part 1 : " << Dijkstra(realCave) << "\n";
    std::cout << "Part 2 : " << Dijkstra(extendedRealCave,true) << "\n";

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}