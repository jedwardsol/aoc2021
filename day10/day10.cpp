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

std::map<char,int> corruptScore
{
    {')',    3},
    {']',   57},
    {'}', 1197},
    {'>',25137}
};


std::map<char,int> incompleteScore
{
    {')',    1},
    {']',    2},
    {'}',    3},
    {'>',    4}
};



void score(std::vector<std::string> const &lines)
{
    auto                    part1Score{0};
    std::vector<int64_t>    part2Scores;

    for(auto line : lines)
    {
        std::stack<char> stack;
        bool             corrupt{false};

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
                    part1Score+=corruptScore[c];
                    corrupt=true;
                    break;
                }
            }
        }

        if(!corrupt)
        {
            int64_t part2Score{};

            while(!stack.empty())
            {
                part2Score*=5;
                part2Score+= incompleteScore[ closer[stack.top()]];
                stack.pop();
            }

            part2Scores.push_back(part2Score);
        }
    }


    std::cout << "Part 1 " << part1Score << "\n";

    std::ranges::sort(part2Scores);


    std::cout << "Part 2 " << part2Scores[part2Scores.size()/2] << "\n";
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

    score(lines);
 
    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}