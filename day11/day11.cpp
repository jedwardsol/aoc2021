#include <cassert>
#include <cstdint>

#include <array>

#include <iostream>
#include <sstream>
#include <format>
#include <string>
#include <chrono>
#include <thread>
using namespace std::literals;


using namespace std::literals;
#include "include/thrower.h"
#include "include/console.h"


extern std::istringstream data;


class Grid
{
    Console console;

public:

    Grid(std::istream &data)   
    {
        console.clear();

        for(int r=0;r<10;r++)
        {
            std::string line;
            std::getline(data,line);

            for(int c=0;c<10;c++)
            {
                grid[r][c]=line[c]-'0';
            }
        }
    }


    void show()
    {
        console.goTo(0,0);
        for(int row=0;row<10;row++)
        {
            for(int column=0;column<10;column++)
            {
                auto octopus = grid[row][column];

                console.grey ( octopus ? octopus * 10 : 100 );

                std::cout << static_cast<char>( octopus ? (octopus + '0') : 219);
            }
            std::cout << "           \n";
        }
        std::cout << '\n';
    }


    int step()
    {
        flashes=0;

        for(int r=0;r<10;r++)
        {
            for(int c=0;c<10;c++)
            {
                increment(r,c);
            }
        }

        for(auto &row : grid)
        {
            for(auto &i : row)
            {
                if(i > 9) 
                {
                    i=0;
                }
            }
        }

        return flashes;
    }


private:

    void increment(int r,int c)
    {
        grid[r][c]++;

        if(grid[r][c] == 10)
        {
            flash(r,c);
        }
    }


    void flash(int r,int c)
    {
        flashes++;


        for(int nr=std::max(r-1,0); nr<=std::min(r+1,9);nr++)
        {
            for(int nc=std::max(c-1,0); nc<=std::min(c+1,9);nc++)
            {
                increment(nr,nc);
            }
        }
    }



public:
    std::array<std::array<char,10>,10> grid;
    int flashes{};
};

int main()
try
{
    Grid grid{data};
        grid.show();

    int totalFlashes{};
    int step{};
    int flashes;
    int part1{};

    do
    {
        flashes=grid.step();
        totalFlashes+=flashes;
        grid.show();
        std::this_thread::sleep_for(20ms);

        step++;
        
        if(step==100)
        {
            part1=totalFlashes;
        }

    }while(flashes<100);

    std::cout << "Part1 " << totalFlashes << "\n";
    std::cout << "Part2 " << step << "\n";

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}