#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <string>

#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"

extern std::istringstream data;

struct Floor
{
    uint8_t height;
    uint8_t basin_;
    bool    lowPoint;


    bool wall() const
    {
        return height==9;
    }
};

using Cave=std::array<std::array<Floor,102>,102>;

void part1(Cave  &cave)
{
    auto count{0};
    auto part1{0};

    for(int row=1;row<=100;row++)
    {
        for(int column=1;column<=100;column++)
        {
            auto       &element=cave[row]  [column];
            auto const &up     =cave[row-1][column];
            auto const &down   =cave[row+1][column];
            auto const &left   =cave[row]  [column-1];
            auto const &right  =cave[row]  [column+1];

            if(   element.height < up.height
               && element.height < down.height
               && element.height < left.height
               && element.height < right.height)
            {
                count++;
                part1+=element.height+1;
                element.basin_=count;
                element.lowPoint=true;
            }
        }
    }

    std::cout << std::format("Part 1 : {}  {} basins\n",part1,count);
}

void part2(Cave  &cave)
{
    bool flooding;
    int passes{};

    do
    {
        flooding=false;
        passes++;
        for(int row=1;row<=100;row++)
        {
            for(int column=1;column<=100;column++)
            {
                std::array<Floor*,5> region{
                &cave[row]  [column],
                &cave[row-1][column],
                &cave[row+1][column],
                &cave[row]  [column-1],
                &cave[row]  [column+1],
                };

                if(   !region[0]->wall()
                   && region[0]->basin_  == 0)
                {
                    flooding=true;

                    uint8_t basin{};

                    for(auto floor : region)
                    {
                        if(   !floor->wall()
                           && floor->basin_  != 0)
                        {
                            basin=floor->basin_;
                        }
                    }

                    if(basin)
                    {
                        for(auto floor : region)
                        {
                            if(   !floor->wall())
                            {
                                floor->basin_=basin;
                            }
                        }
                    }
                }
            }
        }


        for(auto &row : cave)
        {
            for(auto &floor : row)
            {
                std::cout << std::format("{}", floor.wall() ? char(219) : (floor.lowPoint ? '.' : char(' '+!floor.basin_)));
            }
            std::cout << '\n';
        }


    } while(flooding);

    std::cout << passes << '\n';

    int count[256]{};

    for(auto &row : cave)
    {
        for(auto &floor : row)
        {
            if(!floor.wall())
            {
                count[floor.basin_]++;
            }
        }
    }


    for(auto &row : cave)
    {
        for(auto &floor : row)
        {
            std::cout << std::format("{}", floor.wall() ? char(219) : (floor.lowPoint ? '.' : char(' '+floor.basin_)));
        }
        std::cout << '\n';
    }


    assert(count[0]==0);


    std::ranges::sort(count, std::greater<int>{});

    std::cout << std::format("Part 2 : {}\n",count[0]*count[1]*count[2]);

}




int main()
try
{
    Cave   cave{};

    for(int wall=0;wall<102;wall++)
    {
        cave[0]   [wall].height=9;
        cave[101] [wall].height=9;
        cave[wall][0]   .height=9;
        cave[wall][101] .height=9;
    }

    for(int row=1;row<=100;row++)
    {
        std::string line;
        std::getline(data,line);
        assert(data.good());
        assert(line.size()==100);

        for(int column=1;column<=100;column++)
        {
            assert(isdigit(line[column-1]));
            cave[row][column].height=line[column-1]-'0';
        }
    }

    part1(cave);

    part2(cave);


    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}



