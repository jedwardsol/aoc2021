#pragma once
#include "rotation.h"

constexpr Rotation identity
{{
    { 1, 0, 0},
    { 0, 1, 0},
    { 0, 0, 1},
}};

constexpr Rotation rotateX       // roll away
{{
    { 1, 0, 0},
    { 0, 0,-1},
    { 0, 1, 0},
}};

constexpr Rotation rotateY      // spin clockwise   (looking down)
{{
    { 0, 0, 1},
    { 0, 1, 0},
    {-1, 0, 0},
}};

constexpr Rotation rotateZ      // roll to the left
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


                       


// keeping 1 to the right.
constexpr Rotation  rotate_132  {identity};                                                     //  1 to right, 3 on top, 2 to the back
constexpr Rotation  rotate_153  {rotateX};                                                      //  1 to right, 5 on top, 3 to the back
constexpr Rotation  rotate_145  {rotateX * rotateX};                                            //  1 to right, 4 on top, 5 to the back
constexpr Rotation  rotate_124  {rotateX * rotateX * rotateX};                                  //  1 to right, 2 on top, 4 to the back

// putting 2 to the right.
constexpr Rotation  rotate_236  {rotateY};                                                      //  2 to right, 3 on top, 6 to the back
constexpr Rotation  rotate_213  {rotateX * rotateY};                                            //  2 to right, 1 on top, 3 to the back
constexpr Rotation  rotate_241  {rotateX * rotateX * rotateY};                                  //  2 to right, 4 on top, 1 to the back
constexpr Rotation  rotate_264  {rotateX * rotateX * rotateX * rotateY};                        //  2 to right, 6 on top, 4 to the back

// putting 3 to the right.
constexpr Rotation  rotate_362  {rotateZ * rotateZ * rotateZ};                                  //  3 to right, 6 on top, 2 to the back
constexpr Rotation  rotate_356  {rotateX * rotateZ * rotateZ * rotateZ};                        //  3 to right, 5 on top, 6 to the back
constexpr Rotation  rotate_315  {rotateX * rotateX * rotateZ * rotateZ * rotateZ};              //  3 to right, 1 on top, 5 to the back
constexpr Rotation  rotate_321  {rotateX * rotateX * rotateX * rotateZ * rotateZ * rotateZ};    //  3 to right, 2 on top, 1 to the back

// putting 4 to the right.
constexpr Rotation  rotate_412  {rotateZ};                                                      //  4 to right, 1 on top, 2 to the back
constexpr Rotation  rotate_451  {rotateX * rotateZ};                                            //  4 to right, 5 on top, 1 to the back
constexpr Rotation  rotate_465  {rotateX * rotateX * rotateZ};                                  //  4 to right, 6 on top, 5 to the back
constexpr Rotation  rotate_426  {rotateX * rotateX * rotateX * rotateZ};                        //  4 to right, 2 on top, 6 to the back

// putting 5 to the right.
constexpr Rotation  rotate_531  {rotateY * rotateY * rotateY};                                  //  5 to right, 3 on top, 1 to the back
constexpr Rotation  rotate_563  {rotateX * rotateY * rotateY * rotateY};                        //  5 to right, 6 on top, 3 to the back
constexpr Rotation  rotate_546  {rotateX * rotateX * rotateY * rotateY * rotateY};              //  5 to right, 4 on top, 6 to the back
constexpr Rotation  rotate_514  {rotateX * rotateX * rotateX * rotateY * rotateY * rotateY};    //  5 to right, 1 on top, 4 to the back
                                                     
// putting 6 to the right.
constexpr Rotation  rotate_642  {rotateZ * rotateZ};                                            //  6 to right, 4 on top, 2 to the back
constexpr Rotation  rotate_654  {rotateX * rotateZ * rotateZ};                                  //  6 to right, 5 on top, 4 to the back
constexpr Rotation  rotate_635  {rotateX * rotateX * rotateZ * rotateZ};                        //  6 to right, 3 on top, 5 to the back
constexpr Rotation  rotate_623  {rotateX * rotateX * rotateX * rotateZ * rotateZ};              //  6 to right, 2 on top, 4 to the back


constexpr Die rotateDie(Rotation const &rotation)
{
    Die result{initialDie};

    for(auto &face : result)
    {
        face = rotation*face;
    }

    return result;
}


constexpr std::array<Rotation,24>  allRotations
{{
    rotate_132,
    rotate_153,
    rotate_145,
    rotate_124,

    rotate_236,
    rotate_213,
    rotate_241,
    rotate_264,

    rotate_362,
    rotate_356,
    rotate_315,
    rotate_321,

    rotate_412,
    rotate_451,
    rotate_465,
    rotate_426,
          
    rotate_531,
    rotate_563,
    rotate_546,
    rotate_514,

    rotate_642,
    rotate_654,
    rotate_635,
    rotate_623,
}};
