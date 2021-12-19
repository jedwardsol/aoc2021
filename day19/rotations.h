#pragma once
#include "rotation.h"

constexpr Rotation identity
{{
    { 1, 0, 0},
    { 0, 1, 0},
    { 0, 0, 1},
}};

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


// keeping 1 to the right.
constexpr std::array<Rotation,1>  rotate_132  {identity};                                               //  1 to right, 3 on top, 2 to the back
constexpr std::array<Rotation,1>  rotate_153  {rotateX};                                                //  1 to right, 5 on top, 3 to the back
constexpr std::array<Rotation,2>  rotate_145  {rotateX, rotateX};                                       //  1 to right, 4 on top, 5 to the back
constexpr std::array<Rotation,3>  rotate_124  {rotateX, rotateX, rotateX};                              //  1 to right, 2 on top, 4 to the back

// putting 2 to the right.
constexpr std::array<Rotation,1>  rotate_236  {rotateY};                                                //  2 to right, 3 on top, 6 to the back
constexpr std::array<Rotation,2>  rotate_213  {rotateY, rotateX};                                       //  2 to right, 1 on top, 3 to the back
constexpr std::array<Rotation,3>  rotate_241  {rotateY, rotateX, rotateX};                              //  2 to right, 4 on top, 1 to the back
constexpr std::array<Rotation,4>  rotate_264  {rotateY, rotateX, rotateX, rotateX};                     //  2 to right, 6 on top, 4 to the back


// putting 3 to the right.
constexpr std::array<Rotation,3>  rotate_362  {rotateZ, rotateZ, rotateZ};                              //  3 to right, 6 on top, 2 to the back
constexpr std::array<Rotation,4>  rotate_356  {rotateZ, rotateZ, rotateZ, rotateX};                     //  3 to right, 5 on top, 6 to the back
constexpr std::array<Rotation,5>  rotate_315  {rotateZ, rotateZ, rotateZ, rotateX, rotateX};            //  3 to right, 1 on top, 5 to the back
constexpr std::array<Rotation,6>  rotate_321  {rotateZ, rotateZ, rotateZ, rotateX, rotateX, rotateX};   //  3 to right, 2 on top, 1 to the back



