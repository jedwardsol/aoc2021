#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <deque>
#include <map>
#include <string>

#include <fstream>
#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"

/*



  0:      1:      2:      3:      4:
 aaaa    ....    aaaa    aaaa    ....
b    c  .    c  .    c  .    c  b    c
b    c  .    c  .    c  .    c  b    c
 ....    ....    dddd    dddd    dddd
e    f  .    f  e    .  .    f  .    f
e    f  .    f  e    .  .    f  .    f
 gggg    ....    gggg    gggg    ....

  5:      6:      7:      8:      9:
 aaaa    aaaa    aaaa    aaaa    aaaa
b    .  b    .  .    c  b    c  b    c
b    .  b    .  .    c  b    c  b    c
 dddd    dddd    ....    dddd    dddd
.    f  e    f  .    f  e    f  .    f
.    f  e    f  .    f  e    f  .    f
 gggg    gggg    ....    gggg    gggg



 0                                  6 bits
 1  2 bits
 2                          5 bits
 3                          5 bits
 4                  4 bits
 5                          5 bits
 6                                  6 bits
 7          3 bits
 8                                          7 bits
 9                                  6 bits



 5's


       2:          3:          5:  
      aaaa        aaaa        aaaa            
     .    c      .    c      b    .           
     .    c      .    c      b    .
      dddd        dddd        dddd            
     e    .      .    f      .    f           
     e    .      .    f      .    f
      gggg        gggg        gggg            
          
          
          
6's       
          
       0:          6:          9:
      aaaa        aaaa        aaaa            
     b    c      b    .      b    c           
     b    c      b    .      b    c
      ....        dddd        dddd            
     e    f      e    f      .    f           
     e    f      e    f      .    f
      gggg        gggg        gggg            



 */



struct Puzzle
{
    std::vector<std::string> patterns;
    std::vector<std::string> outputs;

    std::map<std::string,int> string_to_number;
    std::map<int,std::string> number_to_string;


    auto associate(std::string const &pattern, int number)
    {
        assert(!string_to_number.contains(pattern));
        assert(!number_to_string.contains(number));

        string_to_number[pattern] = number;
        number_to_string[number]  = pattern;

        assert(string_to_number.size() == number_to_string.size());
    }

    auto answer()
    {
        int answer{};

        for(auto const &output : outputs)
        {
            assert(string_to_number.contains(output));

            answer*=10;
            answer+=string_to_number[output];
        }

        return answer;
    }

    auto numSolved()
    {
        assert(number_to_string.size()==string_to_number.size());
        return number_to_string.size();
    }
};


void part1(std::vector<Puzzle> const &puzzles)
{
    size_t part1{};

    for(auto const &puzzle: puzzles)
    {
        for(auto const &output : puzzle.outputs)
        {
            if(   output.size() == 2
               || output.size() == 3
               || output.size() == 4
               || output.size() == 7)
            {
                part1++;
            }
        }
    }

    std::cout  << part1 << "\n";
}



bool contains(std::string const &target,  std::string const &pattern)
{
    for(auto c : pattern)             
    {
        if(target.find(c) == target.npos)
        {
            return false;
        }
    }

    return true;
}




int solve(Puzzle &puzzle)
{
    // known ones.
    for(auto const &pattern : puzzle.patterns)
    {
        if(   pattern.size() == 2)
        {
            puzzle.associate(pattern,1);
        }
        else if(   pattern.size() == 3)
        {
            puzzle.associate(pattern,7);
        }
        else if(   pattern.size() == 4)
        {
            puzzle.associate(pattern,4);
        }
        else if(   pattern.size() == 7)
        {
            puzzle.associate(pattern,8);
        }
    }
    assert(puzzle.numSolved()==4);


    // find 3 and 6   (3 is the only 5-element digit that contains 1)
    //                (6 is the only 6-element digit that doesn't contain 1)
    for(auto const &pattern : puzzle.patterns)
    {
        if(pattern.size() == 5)
        {
            if( contains(pattern,  puzzle.number_to_string[1]))
            {
                puzzle.associate(pattern,3);
            }
        }
        else if(   pattern.size() == 6)
        {
            if( !contains(pattern,  puzzle.number_to_string[1]))
            {
                puzzle.associate(pattern,6);
            }
        }
    }
    assert(puzzle.numSolved()==6);


    // find 9       (9 is the only 6-element digit that contains 3)
    for(auto const &pattern : puzzle.patterns)
    {
        if(   pattern.size() == 6)
        {
            if(contains(pattern,  puzzle.number_to_string[3]))
            {
                puzzle.associate(pattern,9);
            }
        }
    }
    assert(puzzle.numSolved()==7);

    // find 0       (0 is the remaining 6-element digit)
    for(auto const &pattern : puzzle.patterns)
    {
        if(   pattern.size() == 6)
        {
            if(!puzzle.string_to_number.contains(pattern))
            {
                puzzle.associate(pattern,0);
            }
        }
    }
    assert(puzzle.numSolved()==8);


    // find 2 and 5   (9 contains 5,  but not 2)
    for(auto const &pattern : puzzle.patterns)
    {
        if(   pattern.size() == 5)
        {
            if(!puzzle.string_to_number.contains(pattern) )
            {
                if(contains(puzzle.number_to_string[9],  pattern ))
                {
                    puzzle.associate(pattern,5);
                }
                else 
                {
                    puzzle.associate(pattern,2);
                }
            }
        }
    }
    assert(puzzle.numSolved()==10);

    return puzzle.answer();
}


void part2(std::vector<Puzzle> &puzzles)
{
    int sum{0};

    for(auto &puzzle: puzzles)
    {
        sum+=solve(puzzle);

    }

    std::cout << "Part 2 : " << sum << "\n";
}




int main()
try
{
    std::ifstream       file{"day8.txt"};

    if(!file)
    {
        throw_system_error("Failed to open file");
    }

    std::string             rawline;

    std::vector<Puzzle>     puzzles;

    while(std::getline(file ,rawline))
    {
        std::istringstream  line{rawline};
        Puzzle              puzzle;

        for(int i=0;i<10;i++)
        {
            puzzle.patterns.emplace_back();
            line >> puzzle.patterns.back();
            std::ranges::sort(puzzle.patterns.back());
        }


        std::string pipe;
        line >> pipe;
        assert(pipe == "|"s);

        for(int i=0;i<4;i++)
        {
            puzzle.outputs.emplace_back();
            line >> puzzle.outputs.back();
            std::ranges::sort(puzzle.outputs.back());
        }

        puzzles.push_back(puzzle);
    }

    part1(puzzles);
    part2(puzzles);

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}