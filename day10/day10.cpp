#include <cassert>
#include <cstdint>

#include <map>
#include <vector>
#include <stack>
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


bool opener(char c)
{
    return     c=='(' 
           ||  c=='[' 
           ||  c=='{' 
           ||  c=='<'; 
}


std::map<char,char> closer
{
    {'(',')'},
    {'[',']'},
    {'{','}'},
    {'<','>'}
};

std::map<char,int> score
{
    {')',    3},
    {']',   57},
    {'}', 1197},
    {'>',25137}
};



void part1(std::vector<std::string> const &lines)
{
    auto totalScore{0};

    for(auto line : lines)
    {
        std::stack<char> stack;

        for(auto c : line)
        {
            if(opener(c))
            {
                stack.push(c);
            }
            else
            {
                if(closer[stack.top()] == c)
                {
                    stack.pop();
                }
                else
                {
                    totalScore+=score[c];
                    break;
                }
            }
        }
    }


    std::cout << "Part 1 " << totalScore << "\n";
}


int main()
try
{
    auto lines = std::vector<std::string>{};
    auto line  = std::string{};

    while(std::getline(data ,line))
    {
        lines.push_back(line);
    }

    part1(lines);
 
    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}