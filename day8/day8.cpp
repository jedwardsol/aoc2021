#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <deque>
#include <map>

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
  aaaa        aaaa        aaaa                3
 .    c      .    c      b    .              1  2
 .    c      .    c      b    .
  dddd        dddd        dddd                3 
 e    .      .    f      .    f              1  2
 e    .      .    f      .    f
  gggg        gggg        gggg                3
          
          
          
6's       
          
   0:          6:          9:
  aaaa        aaaa        aaaa                3
 b    c      b    .      b    c              3  2
 b    c      b    .      b    c
  ....        dddd        dddd                2
 e    f      e    f      .    f              2  3 
 e    f      e    f      .    f
  gggg        gggg        gggg                3



 */



struct Puzzle
{
    std::vector<std::string> patterns;
    std::vector<std::string> outputs;

    std::map<std::string,int> string_to_number;
    std::map<int,std::string> number_to_string;
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


auto associate(Puzzle &puzzle, std::string const &pattern, int number)
{
    assert(!puzzle.string_to_number.contains(pattern));
    assert(!puzzle.number_to_string.contains(number));



    puzzle.string_to_number[pattern] = number;
    puzzle.number_to_string[number]  = pattern;


    assert(puzzle.string_to_number.size() == puzzle.number_to_string.size());

}


int solve(Puzzle &puzzle)
{
    // known ones.
    for(auto const &pattern : puzzle.patterns)
    {
        if(   pattern.size() == 2)
        {
            associate(puzzle,pattern,1);
        }
        else if(   pattern.size() == 3)
        {
            associate(puzzle,pattern,7);
        }
        else if(   pattern.size() == 4)
        {
            associate(puzzle,pattern,4);
        }
        else if(   pattern.size() == 7)
        {
            associate(puzzle,pattern,8);
        }
    }
    assert(puzzle.number_to_string.size()==4);
    assert(puzzle.string_to_number.size()==4);


    // find 3 and 6   (3 is the only 5-element digit that contains 1)
    //                (6 is the only 6-element digit that doesn't contains 1)
    for(auto const &pattern : puzzle.patterns)
    {
        if(   pattern.size() == 5)
        {
            if( contains(pattern,  puzzle.number_to_string[1]))
            {
                associate(puzzle,pattern,3);
            }
        }
        else if(   pattern.size() == 6)
        {
            if( !contains(pattern,  puzzle.number_to_string[1]))
            {
                associate(puzzle,pattern,6);
            }
        }
    }
    assert(puzzle.number_to_string.size()==6);
    assert(puzzle.string_to_number.size()==6);


    // find 9       (9 is the only 6-element digit that contains 3)
    for(auto const &pattern : puzzle.patterns)
    {
        if(   pattern.size() == 6)
        {
            if(contains(pattern,  puzzle.number_to_string[3]))
            {
                associate(puzzle,pattern,9);
            }
        }
    }
    assert(puzzle.number_to_string.size()==7);
    assert(puzzle.string_to_number.size()==7);

    // find 0       (0 is the remaining 6-element digit)
    for(auto const &pattern : puzzle.patterns)
    {
        if(   pattern.size() == 6)
        {
            if(!puzzle.string_to_number.contains(pattern))
            {
                associate(puzzle,pattern,0);
            }
        }
    }
    assert(puzzle.number_to_string.size()==8);
    assert(puzzle.string_to_number.size()==8);


// distinguish 2 and 5   (9 contains 5,  but not 2)

    for(auto const &pattern : puzzle.patterns)
    {
        if(   pattern.size() == 5)
        {
            if(!puzzle.string_to_number.contains(pattern) )
            {
                if(contains(puzzle.number_to_string[9],  pattern ))
                {
                    associate(puzzle,pattern,5);
                }
                else 
                {
                    associate(puzzle,pattern,2);
                }
            }
        }
    }
    assert(puzzle.number_to_string.size()==10);
    assert(puzzle.string_to_number.size()==10);


    int answer{};

    for(auto const &output : puzzle.outputs)
    {
        answer*=10;
        answer+=puzzle.string_to_number[output];
    }

    return answer;

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

        std::ranges::sort(puzzle.patterns, [](auto const &s1, auto const &s2){return s1.size() < s2.size();});

        std::string dummy;
        line >> dummy;

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