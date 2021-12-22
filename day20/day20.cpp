#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <bitset>
#include <memory>
#include <string>

#include <iostream>
#include <sstream>
#include <format>


using namespace std::literals;
#include "include/thrower.h"

extern std::istringstream testData;
extern std::istringstream realData;


template <int N>
struct Canvas
{
    Canvas()
    {}

    bool inRange(int row, int column) const
    {
        return    row >= minRow
               && row <= maxRow
               && column >= minColumn
               && column <= maxColumn;
    }

    bool read(int row, int column)  const
    {
        if(inRange(row,column))
        {
            row   +=center;
            column+=center;

            return canvas.at( (row*N)+column);
        }
        else
        {
            return background;
        }
    }

    void write(int row, int column,bool value)
    {
        minRow    = std::min(minRow,   row);
        maxRow    = std::max(maxRow,   row);
        minColumn = std::min(minColumn,row);
        maxColumn = std::max(maxColumn,row);

        row   +=center;
        column+=center;

        canvas.at( (row*N)+column) = value;
    }


    void draw() const
    {
        for(int row = minRow; row <= maxRow; row++)
        {
            for(int column = minColumn; column <= maxColumn; column++)
            {
                std::cout << (read(row,column) ? '#' : ' ');
            }

            std::cout << "\n";
        }
    }

    int count() const
    {
        int count{};

        for(int row = minRow; row <= maxRow; row++)
        {
            for(int column = minColumn; column <= maxColumn; column++)
            {
                if(read(row,column))
                {
                    count++;
                }
            }
        }

        return count;
    }

    constexpr static int    size  =N;
    constexpr static int    center=N/2;
    
    std::array<bool,N*N>    canvas{};

    int                     minRow   {std::numeric_limits<int>::max()};
    int                     maxRow   {std::numeric_limits<int>::min()};

    int                     minColumn{std::numeric_limits<int>::max()};
    int                     maxColumn{std::numeric_limits<int>::min()};
    bool                    background{};
};




auto readCanvas()
{
    auto canvas1 = std::make_shared<Canvas<400>>();
    auto canvas2 = std::make_shared<Canvas<400>>();

    std::istringstream &data{realData};

    std::string enhancement;

    std::getline(data,enhancement);
    assert(enhancement.size()==512);

    std::string line;

    int row{};
    
    while(std::getline(data,line))
    {
        if(line.empty())
        {
            continue;
        }

        int column{};

        for(auto c : line)
        {
            canvas1->write(row,column, c=='#');
            column++;
        }

        row++;
    }

    canvas2->background=true;


    return std::make_tuple(enhancement,canvas1, canvas2);
}



int getPixel(auto const &canvas, int centerRow, int centerColumn)
{
    int pixel{};

    for(int row=centerRow-1; row<=centerRow+1; row++)
    {
        for(int column=centerColumn-1; column<=centerColumn+1; column++)
        {
            pixel <<= 1;
            pixel |= +canvas.read(row,column);
        }
    }

    return pixel;
}


void enhance(std::string const &enhancement, auto &canvas1, auto &canvas2)
{
    for(int row = canvas1.minRow-1; row <= canvas1.maxRow+1; row++)
    {
        for(int column = canvas1.minColumn-1; column <= canvas1.maxColumn+1; column++)
        {
            int pixel=getPixel(canvas1,row,column);
            assert(pixel < 512);

            canvas2.write(row,column,  enhancement[pixel] == '#');                            
        }
    }
}





int main()
try
{
    auto [enhancement, canvas1, canvas2] = readCanvas();


    for(int i=1; i<=50;i++)
    {
        enhance(enhancement, *canvas1, *canvas2);
        std::swap(canvas1,canvas2);

        if(i==2)
        {
            std::cout << std::format("Part 1 : {}\n",canvas1->count());
        }

        if(i==50)
        {
            std::cout << std::format("Part 2 : {}\n",canvas1->count());
        }
    }

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}