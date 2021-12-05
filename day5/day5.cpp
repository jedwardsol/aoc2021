#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <utility>
#include <cstdio>
#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <format>
#include <thread>
#include <chrono>

using namespace std::literals;
#include "include/thrower.h"

struct Point
{
    int x;
    int y;
};

struct Line
{
    Point start;
    Point end;

    auto dy() const
    {
             if(start.y == end.y) return  0;
        else if(start.y <  end.y) return  1;
        else                      return -1;
    }

    auto dx() const
    {
             if(start.x == end.x) return  0;
        else if(start.x <  end.x) return  1;
        else                      return -1;
    }

    auto horizontal()  const
    {
        return dx()==0;
    }

    auto vertical() const
    {
        return dy()==0;
    }

    auto diagonal() const
    {
        return !horizontal() && !vertical();
    }

    auto len() const
    {
        return std::max( std::abs(start.x-end.x),  std::abs(start.y-end.y));
    }
};

using Grid = std::array<std::array<uint8_t,1000>,1000>;



auto readData()
{
    auto data = std::ifstream{"data.txt"};

    if(!data)
    {
        throw_system_error("data.txt");
    }

    std::string                 line;
    std::vector<std::string>    lines;

    while(std::getline(data,line))
    {
        lines.push_back(line);
    }

    return lines;        
}


auto parseData(std::vector<std::string> const &data)
{
    std::vector<Line>  lines;

    for(auto const &string : data)
    {
        Line line{};

        auto parsed = std::sscanf(string.c_str(),"%d,%d -> %d,%d",
                                  &line.start.x,
                                  &line.start.y,
                                  &line.end.x,
                                  &line.end.y);

        if(parsed!=4)
        {
            throw_runtime_error("parse failure " + string);
        }

        lines.push_back(line);
    }

    return lines;
}


void countOverlaps(Grid &grid)
{
    int overlapCount=0;

    for(auto const &row : grid)
    {
        for(auto element : row)
        {
            if(element > 1)
            {
                overlapCount++;
            }
        }
    }

     std::cout << "overlapCount is " << overlapCount << "\n";
}


void go(std::vector<Line> const &lines, bool skipDiagonals)
{
    auto grid  = std::make_unique<Grid>();

    for(auto line:lines)
    {
        if(skipDiagonals && line.diagonal())
        {
            continue;
        }

        for(int x=line.start.x, y=line.start.y, i=0 ;
                i<=line.len();
                x+=line.dx(), y+=line.dy(), i++)
        {
            grid->at(x).at(y)++;
        }
    }

    countOverlaps(*grid);
}


void part1(std::vector<Line> const &lines)
{
    go(lines,true);
}

void part2(std::vector<Line> const &lines)
{
    go(lines,false);
}



int main()
try
{
    auto data  = readData();
    auto lines = parseData(data);

    part1(lines);
    part2(lines);


    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}