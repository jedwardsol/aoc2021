#define NOMINMAX
#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <string>
#include <tuple>

#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"

extern std::istringstream &data;

constexpr auto block = static_cast<char>(219);

struct Pixel
{
    size_t column, row;
};

struct Fold
{
    enum class Axis
    {
        vertical,
        horizontal
    };

    Axis    axis;
    size_t  offset;
};



auto parseInput()
{
    std::vector<Pixel>  pixels;
    std::vector<Fold>   folds;
    std::string         line;


    int maxRow{};
    int maxColumn{};

    while(std::getline(data,line))
    {
        if(line.empty())
        {
            continue;
        }
        
        auto comma =line.find(',');
        auto equals=line.find('=');

        if(comma != line.npos)
        {
            auto column=std::stoi(line);   
            auto row   =std::stoi(line.substr(comma+1));    

            maxColumn=std::max(maxColumn,column);
            maxRow   =std::max(maxRow,   row);

            pixels.emplace_back(column,row);
        }
        else
        {
            assert(equals!=line.npos);

            auto offset   =std::stoi(line.substr(equals+1));    
            auto axis     =line[equals-1];    

            assert(axis=='x' || axis=='y');

            folds.emplace_back( axis=='x' ? Fold::Axis::vertical : Fold::Axis::horizontal, offset);
        }
    }

    assert(maxRow    < 1000);
    assert(maxColumn < 1500);

    return std::make_pair(pixels,folds);
}


void trim(std::vector<std::string> &grid,size_t maxRow, size_t maxColumn)
{
    grid.resize(maxRow+1);
    for(auto &row : grid)
    {
        row.resize(maxColumn+1);
    }
}

void draw(std::vector<std::string> const &grid)
{
    for(auto row : grid)
    {
        std::cout << row << '\n';
    }
}

auto count(std::vector<std::string> const &grid)
{
    size_t count{};

    for(auto row : grid)
    {
        count+=std::ranges::count_if(row, [](auto c) { return c==block;});
    }

    return count;
}


void fold(std::vector<std::string> &grid, Fold const &fold)
{
    if(fold.axis == Fold::Axis::horizontal)
    {
        auto const height = std::min( fold.offset,   grid.size()-fold.offset-1);

        for(auto pixel = 0 ; pixel < height; pixel++)
        {
            auto const top    = fold.offset - 1 - pixel;
            auto const bottom = fold.offset + 1 + pixel;

            assert(grid[top].size() == grid[bottom].size());

            for(auto column = 0 ; column < grid[top].size(); column++)
            {
                if(grid[bottom][column] == block)
                {
                    grid[top][column]=block;
                }
            }
        }

        grid.resize(fold.offset);
    }
    else
    {
        auto const width = std::min( fold.offset,   grid[0].size()-fold.offset-1);

        for(auto &row : grid)
        {
            for(auto pixel = 0 ; pixel < width; pixel++)
            {
                auto const left  = fold.offset - 1 - pixel;
                auto const right = fold.offset + 1 + pixel;

                if(row[right] == block)
                {
                    row[left]=block;
                }
            }

            row.resize(fold.offset);
        }
    }
}


int main()
try
{
    auto [pixels, folds] = parseInput();
    auto grid            = std::vector ( 1000, std::string(1500,' '));

    size_t maxRow{};
    size_t maxColumn{};

    for(auto const &pixel : pixels)
    {
        maxColumn=std::max(maxColumn,pixel.column);
        maxRow   =std::max(maxRow,   pixel.row);

        grid[pixel.row][pixel.column]=block;
    }

    trim(grid,maxRow,maxColumn);
    assert(count(grid)==pixels.size());

    for(auto const &fold : folds)
    {
        ::fold(grid, fold);
        std::cout << "Pixels : " << count(grid) << '\n';
    }

    draw(grid);

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}