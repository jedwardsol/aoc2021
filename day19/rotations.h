#pragma once
#include "rotation.h"

constexpr Rotation rotateX
{{
    { 1, 0, 0},
    { 0, 0,-1},
    { 0, 1, 0},
}};

constexpr Rotation rotateY
{{
    { 0, 0, 1},
    { 0, 1, 0},
    {-1, 0, 0},
}};

constexpr Rotation rotateZ
{{
    { 0,-1, 0},
    { 1, 0, 0},
    { 0, 0, 1},
}};






///

/*

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


    +x = 1
    +y = 3
    +z = 2
    
*/

//                            x  y  z   
constexpr Location initial1 { 1, 0, 0};
constexpr Location initial2 { 0, 0, 1};
constexpr Location initial3 { 0, 1, 0};
constexpr Location initial4 { 0,-1, 0};
constexpr Location initial5 { 0, 0,-1};
constexpr Location initial6 {-1, 0, 0};


using Die = std::array<Location,6>;

constexpr Die initialDie = 
{{
    initial1,
    initial2,
    initial3,
    initial4,
    initial5,
    initial6
}};


template <size_t N>
constexpr Die rotateDie(std::array<Rotation,N> const &rotations)
{
    Die result{initialDie};

    for(auto &face : result)
    {
        for(auto const &rotation : rotations)
        {
            face = rotation*face;
        }
    }

    return result;
}




constexpr std::array<Rotation,1>  rotate_153  {rotateX};                         //  1 to right, 5 on top, 3 to the back
constexpr std::array<Rotation,2>  rotate_145  {rotateX, rotateX};                //  1 to right, 4 on top, 5 to the back
constexpr std::array<Rotation,3>  rotate_124  {rotateX, rotateX, rotateX};       //  1 to right, 2 on top, 4 to the back







