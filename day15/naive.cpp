#include <cassert>
#include <cstdint>

#include <vector>


#include "include/thrower.h"
#include "include/console.h"

#include "day15.h"




int downRightOnly(Cave  const &cave)
{
    auto const  size=static_cast<int>(cave.size());
    auto const  last=size-1;

    auto        total(std::vector(size, std::vector(size,0)));

    total[last][last]=cave[last][last];

    for(auto row=last-1; row>=0;row--)
    {
        total[row][last] = cave[row][last] + total[row+1][last];
    }

    for(auto col=last-1; col>=0;col--)
    {
        total[last][col] = cave[last][col] + total[last][col+1];
    }


    for(auto row=last-1; row>=0;row--)
    {
        for(auto col=last-1; col>=0;col--)
        {
            total[row][col] = cave[row][col] + std::min(total[row  ][col+1],
                                                        total[row+1][col  ]);
        }
    }

//  printSmall(cave);
//  printSmall(total);

    return total[0][0]-cave[0][0];
}
