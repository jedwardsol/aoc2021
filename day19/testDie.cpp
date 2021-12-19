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
    constexpr  auto die_153 = rotateDie(rotate_153);
    constexpr  auto die_145 = rotateDie(rotate_145);
    constexpr  auto die_124 = rotateDie(rotate_124);

    printDie();
    printDie(initialDie,"Initial    1 to right, 3 on top, 2 to the back");
    printDie(die_153,   "           1 to right, 5 on top, 3 to the back");
    printDie(die_145,   "           1 to right, 4 on top, 5 to the back");
    printDie(die_124,   "           1 to right, 2 on top, 4 to the back");


}