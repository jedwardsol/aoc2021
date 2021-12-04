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


char mostCommon(std::vector<std::string> const &lines, int position)
{
    int oneCount{};
    
    for(auto const &line : lines)
    {
        if(line[position]=='1')
        {
            oneCount++;
        }
    }

    auto zeroCount = lines.size()-oneCount;


    return oneCount >= zeroCount ? '1' : '0';
}

char leastCommon(std::vector<std::string> const &lines, int position)
{
    return mostCommon(lines,position) == '1' ? '0' : '1';
}


void part1(std::vector<std::string> const &lines)
{
    int         gamma{};                    // most common bit
    int         epsilon{};                  // least common bit

    for(int bit=0;bit<12;bit++)
    {
        if(mostCommon(lines,bit) == '1')
        {
            gamma |= 1 << (11-bit);
        }
        else
        {
            epsilon |= 1 << (11-bit);
        }
    }

    std::cout << std::format("Part 1 : {}*{}={}\n",gamma,epsilon,gamma*epsilon);
}


void filter(std::vector<std::string> &lines,int position, char keep)
{
    if(lines.size()==1)
    {
        return;            
    }

    auto newEnd = std::remove_if(std::begin(lines),
                                 std::end(lines),
                                 [&](auto const &line)
                                 {
                                    return line[position]!=keep;
                                 });

    lines.erase(newEnd,std::end(lines));
   
}


void part2(std::vector<std::string> const &lines)
{
    auto  oxygenCandidates{lines};
    auto  co2Candidates   {lines};

    for(int bit=0;bit<12;bit++)
    {
        filter(oxygenCandidates,bit,mostCommon(oxygenCandidates,bit));
        filter(co2Candidates,   bit,leastCommon(co2Candidates,bit));
    }

    if(   oxygenCandidates.size() != 1
       || co2Candidates.size()    != 1)
    {
        throw_runtime_error("Failed to reduce to a single candidate");
    }


    auto  oxygenLevel{0};
    auto  co2Level   {0};

    for(int bit=0;bit<12;bit++)
    {
        if(oxygenCandidates[0][bit] == '1')
        {
            oxygenLevel |= 1 << (11-bit);
        }

        if(co2Candidates[0][bit] == '1')
        {
            co2Level |= 1 << (11-bit);
        }
    }

    std::cout << std::format("Part 2 : {}*{}={}\n",oxygenLevel,co2Level,oxygenLevel*co2Level);

    return ;
}



int main()
try
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
        if(line.size()!=12)
        {
            throw_runtime_error("Wrong length : " + line);
        }

        lines.push_back(line);
    }

    part1(lines);
    part2(lines);

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}