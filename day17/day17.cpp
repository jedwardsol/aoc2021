#include <cassert>
#include <cstdint>


#include <iostream>
#include <sstream>
#include <format>


using namespace std::literals;
#include "include/thrower.h"


/*

                 +y
                 ^
                 |
       -x <------+------> +x
                 |
                 V     ###
                 -y


    target area: x=185..221, y=-122..-74

*/


namespace Target
{
    constexpr int left{185};
    constexpr int right{221};

    constexpr int bottom{-122};
    constexpr int top{ -74};
} ;




struct Position
{
    int x;
    int y;
};

struct Velocity
{
    int dx;
    int dy;
};      



bool inTarget(Position const position)
{
    return    position.x >= Target::left
           && position.x <= Target::right
           && position.y >= Target::bottom
           && position.y <= Target::top;
}

bool beyondTarget(Position const position)
{
    //  assumes source is above left of target

    return    position.x >= Target::right
           || position.y <= Target::bottom;
}


void step(Position &position, Velocity &velocity)
{
    position.x += velocity.dx;
    position.y += velocity.dy;

    velocity.dx = std::max(0, velocity.dx-1);
    velocity.dy--;

}


int heightOfTrajectory(Velocity velocity)
{
    Position    position{};
    Position    highestPosition{};

    while(!beyondTarget(position))
    {
        step(position,velocity);

        if(velocity.dy == 0)
        {
            highestPosition=position;
        }

        if(inTarget(position))
        {
            return highestPosition.y;
        }
    }

    // missed;
    return -1;
}


bool trajectoryHits(Velocity velocity)
{
    Position    position{};

    while(!beyondTarget(position))
    {
        step(position,velocity);

        if(inTarget(position))
        {
            return true;
        }
    }

    return false;
}



void part1()
{
    int highestTrajectory{-1};


    for(int dy=1;dy<= std::abs(Target::bottom); dy++)
    {
        // TODO minimum dx = is smallest triangular number equal or bigger than target::right


        for(int dx=1;dx<= std::abs(Target::right); dx++)
        {
            highestTrajectory = std::max(highestTrajectory, heightOfTrajectory({dx,dy}));
        }
    }

    std::cout << "Part 1 : " << highestTrajectory << "\n";

}


void part2()
{
    int hitCount{};


    for(int dy=Target::bottom;dy<= std::abs(Target::bottom); dy++)
    {

        for(int dx=1;dx<= std::abs(Target::right); dx++)
        {
            if(trajectoryHits({dx,dy}))
            {
                hitCount++;
            }
        }
    }

    std::cout << "Part 2 : " << hitCount << "\n";

}



int main()
try
{
    part1();
    part2();



    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}