#include <cassert>
#include <cstdint>

#include <array>
#include <vector>

#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"

#include "rotations.h"

void printDie()
{

    std::cout << R"(

    Y 
    ^
    | Z
    |/
    -----> X


Start with die

          2 
      +-----+
     /  3  /|
    +-----+ |
    |     |1| 
 6  |  5  | +
    |     |/
    +-----+ 
       4

=================================

)";

}


void printDie(Die const &die, std::string const &name)
{
    std::cout << std::format("{}\n", name);
    for(int face=0;face<6;face++)
    {
        std::cout << std::format("  {} : {:2},{:2},{:2}\n", face+1, die[face][0],die[face][1],die[face][2]);
    }
    std::cout << "\n";            
}

void testDie()
{
    constexpr  auto die_132 = rotateDie(rotate_132);
    constexpr  auto die_153 = rotateDie(rotate_153);
    constexpr  auto die_145 = rotateDie(rotate_145);
    constexpr  auto die_124 = rotateDie(rotate_124);

    constexpr  auto die_236 = rotateDie(rotate_236);
    constexpr  auto die_213 = rotateDie(rotate_213);
    constexpr  auto die_241 = rotateDie(rotate_241);
    constexpr  auto die_264 = rotateDie(rotate_264);

    constexpr  auto die_362 = rotateDie(rotate_362);
    constexpr  auto die_356 = rotateDie(rotate_356);
    constexpr  auto die_315 = rotateDie(rotate_315);
    constexpr  auto die_321 = rotateDie(rotate_321);

    constexpr  auto die_412 = rotateDie(rotate_412);
    constexpr  auto die_451 = rotateDie(rotate_451);
    constexpr  auto die_465 = rotateDie(rotate_465);
    constexpr  auto die_426 = rotateDie(rotate_426);

    constexpr  auto die_531 = rotateDie(rotate_531);
    constexpr  auto die_563 = rotateDie(rotate_563);
    constexpr  auto die_546 = rotateDie(rotate_546);
    constexpr  auto die_514 = rotateDie(rotate_514);

    constexpr  auto die_642 = rotateDie(rotate_642);
    constexpr  auto die_654 = rotateDie(rotate_654);
    constexpr  auto die_635 = rotateDie(rotate_635);
    constexpr  auto die_623 = rotateDie(rotate_623);


    printDie();
    printDie(initialDie,"Initial    1 to right, 3 on top, 2 to the back");
    printDie(die_132,   "           1 to right, 3 on top, 2 to the back");
    printDie(die_153,   "           1 to right, 5 on top, 3 to the back");
    printDie(die_145,   "           1 to right, 4 on top, 5 to the back");
    printDie(die_124,   "           1 to right, 2 on top, 4 to the back");

    std::cout<<"---\n";
    printDie(die_236,   "           2 to right, 3 on top, 6 to the back");
    printDie(die_213,   "           2 to right, 1 on top, 3 to the back");
    printDie(die_241,   "           2 to right, 4 on top, 1 to the back");
    printDie(die_264,   "           2 to right, 6 on top, 6 to the back");

    std::cout<<"---\n";
    printDie(die_362,   "           3 to right, 6 on top, 2 to the back");
    printDie(die_356,   "           3 to right, 5 on top, 6 to the back");
    printDie(die_315,   "           3 to right, 1 on top, 5 to the back");
    printDie(die_321,   "           3 to right, 2 on top, 1 to the back");

    std::cout<<"---\n";
    printDie(die_412,   "           4 to right, 1 on top, 2 to the back");
    printDie(die_451,   "           4 to right, 5 on top, 1 to the back");
    printDie(die_465,   "           4 to right, 6 on top, 5 to the back");
    printDie(die_426,   "           4 to right, 2 on top, 6 to the back");

    std::cout<<"---\n";
    printDie(die_531,   "           5 to right, 3 on top, 1 to the back");
    printDie(die_563,   "           5 to right, 6 on top, 3 to the back");
    printDie(die_546,   "           5 to right, 4 on top, 6 to the back");
    printDie(die_514,   "           5 to right, 1 on top, 4 to the back");

    std::cout<<"---\n";
    printDie(die_642,   "           6 to right, 4 on top, 2 to the back");
    printDie(die_654,   "           6 to right, 5 on top, 4 to the back");
    printDie(die_635,   "           6 to right, 3 on top, 5 to the back");
    printDie(die_623,   "           6 to right, 2 on top, 3 to the back");
}