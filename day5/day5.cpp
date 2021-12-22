#define _CRT_SECURE_NO_WARNINGS
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
#include <iterator>
#include <concepts>
#include <compare>

using namespace std::literals;
#include "include/thrower.h"

struct Point
{
    int x;
    int y;

    [[nodiscard]] bool                 operator== (Point const &) const noexcept = default;
};


class LineIterator 
{
public:
    using iterator_concept  = std::forward_iterator_tag;
    using iterator_category = std::forward_iterator_tag;

    using size_type         = std::size_t;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Point;
    using reference         = value_type const &;
    using pointer           = value_type const *;  

    LineIterator() : p{}, dx{}, dy{}
    {}

    LineIterator(Point p,int dx,int dy) : p{p}, dx{dx}, dy{dy}
    {}

    LineIterator(LineIterator const  &   ) noexcept =default;
    LineIterator(LineIterator        &&  ) noexcept =default;
 
    LineIterator &operator=(LineIterator const  &   ) noexcept =default;
    LineIterator &operator=(LineIterator        &&  ) noexcept =default;

    LineIterator   operator++(int) noexcept
    {
        auto original=*this;
        p.x+=dx;
        p.y+=dy;
        return original;    
    }

    LineIterator   &operator++() noexcept
    {
        p.x+=dx;
        p.y+=dy;
        return *this;    
    }

    value_type operator*() const noexcept
    {
        return p;
    }


public:

    [[nodiscard]] bool                 operator== (LineIterator const &) const noexcept = default;

private:
    Point   p;
    int     dx;
    int     dy;
};




static_assert( std::forward_iterator <LineIterator>);




struct Line
{
private:
    Point pstart;
    Point pend;

public:

    Line(Point start, Point end) noexcept : pstart{start}, pend{end}
    {}

    auto dy() const noexcept
    {
             if(pstart.y == pend.y) return  0;
        else if(pstart.y <  pend.y) return  1;
        else                        return -1;
    }

    auto dx() const noexcept
    {
             if(pstart.x == pend.x) return  0;
        else if(pstart.x <  pend.x) return  1;
        else                        return -1;
    }

    auto diagonal() const noexcept
    {
        return dy()!=0 && dx()!=0;
    }

    auto begin() const noexcept
    {
        return LineIterator{pstart,dx(),dy()};
    }

    auto end() const noexcept
    {
        return LineIterator{ {pend.x+dx(), pend.y+dy()}, dx(), dy()};
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
        Point start,end;

        auto parsed = std::sscanf(string.c_str(),"%d,%d -> %d,%d",
                                  &start.x,
                                  &start.y,
                                  &end.x,
                                  &end.y);

        if(parsed!=4)
        {
            throw_runtime_error("parse failure " + string);
        }

        Line line{start,end};

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

        for(auto point : line)
        {
            grid->at(point.x).at(point.y)++;
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