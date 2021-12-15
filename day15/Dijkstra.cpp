#define NOMINMAX
#include <cassert>
#include <cstdint>

#include <vector>
#include <queue>

#include <limits>


#include "include/thrower.h"
#include "include/console.h"

#include "day15.h"


constexpr auto infinity = std::numeric_limits<int>::max();

struct Location
{
    int row;
    int column;
};

struct Node
{
    int         distance{infinity};
    Location    location{};
//  Location    previous{};         // only needed if we need the route
    bool        visited{false};
};



struct NodeCompare
{
    bool operator()(Node const &lhs, Node const &rhs)
    {
        return lhs.distance > rhs.distance;
    }
} ;

using PriorityQueue = std::priority_queue<Node,std::vector<Node>,NodeCompare>;


auto getNeighbours(int size, Location current)
{
    std::vector<Location>   neighbours;

    if(current.row > 0)
    {
        neighbours.emplace_back(current.row-1,current.column);
    }

    if(current.row < size-1)
    {
        neighbours.emplace_back(current.row+1,current.column);
    }

    if(current.column > 0)
    {
        neighbours.emplace_back(current.row,current.column-1);
    }

    if(current.column < size-1)
    {
        neighbours.emplace_back(current.row,current.column+1);
    }

    return neighbours;
}


int Dijkstra(Cave  const &cave)
{
    auto const      size=static_cast<int>(cave.size());
    auto            queue{PriorityQueue{}};
    auto            graph(std::vector(size, std::vector(size,Node{})));


    for(auto row=0;row<size;row++)
    {
        for(auto column=0;column<size;column++)
        {
            graph[row][column].location=Location{row,column};
        }
    }

    graph[0][0].distance=0;
    queue.push(graph[0][0]);

    while(   !graph[size-1][size-1].visited
          && !queue.empty())
    {
        auto const current=queue.top();
        queue.pop();

        if(graph[current.location.row][current.location.column].visited)
        {
            continue;
        }

        graph[current.location.row][current.location.column].visited=true;

        auto neighbours = getNeighbours(size,current.location);

        for(auto neighbour : neighbours)
        {
            if(! graph[neighbour.row][neighbour.column].visited)
            {
                auto newDistance = current.distance + cave[neighbour.row][neighbour.column];

                if(newDistance < graph[neighbour.row][neighbour.column].distance)
                {
                    graph[neighbour.row][neighbour.column].distance = newDistance;
//                  graph[neighbour.row][neighbour.column].previous = current.location;
                    queue.push(graph[neighbour.row][neighbour.column]);
                }
            }
        }
    }

    return graph[size-1][size-1].distance;
}



