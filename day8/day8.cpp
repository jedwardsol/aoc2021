#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <deque>

#include <fstream>
#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"


struct Line
{
    std::vector<std::string> patterns;
    std::vector<std::string> outputs;
};


int main()
try
{
    std::ifstream       file{"day8.txt"};
    std::string         rawline;

    std::vector<Line>   lines;

    while(std::getline(file ,rawline))
    {
        std::istringstream  line{rawline};

        Line entry;

        for(int i=0;i<10;i++)
        {
            entry.patterns.emplace_back();
            line >> entry.patterns.back();
        }

        std::string dummy;
        line >> dummy;

        for(int i=0;i<4;i++)
        {
            entry.outputs.emplace_back();
            line >> entry.outputs.back();
        }

    

        lines.push_back(entry);
    }

    int part1{};

    for(auto const &line : lines)
    {
        for(auto const &output : line.outputs)
        {
            if(   output.size() == 2
               || output.size() == 4
               || output.size() == 3
               || output.size() == 7)
            {
                part1++;
            }
        }
    }

    std::cout  << part1 << "\n";


    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}