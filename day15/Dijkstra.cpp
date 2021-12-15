#define NOMINMAX
#include <cassert>
#include <cstdint>

#include <vector>
#include <queue>
#include <fstream>

#include <limits>


#include "include/thrower.h"
#include "include/console.h"

#include "day15.h"


constexpr auto infinity = std::numeric_limits<Risk>::max();

struct Location
{
    int row;
    int column;

    bool operator==(Location const&rhs) const noexcept = default;

};

struct Node
{
    Risk        totalRisk{infinity};
    Location    location{};
    Location    previous{};         // only needed if we need the route
    bool        visited{false};
    bool        onPath{false};
};



struct NodeCompare
{
    bool operator()(Node const &lhs, Node const &rhs)
    {
        return lhs.totalRisk > rhs.totalRisk;
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


int Dijkstra(Cave  const &cave, bool save)
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

    graph[0][0].totalRisk=0;
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
                auto newTotalRisk = current.totalRisk + cave[neighbour.row][neighbour.column];

                if(newTotalRisk < graph[neighbour.row][neighbour.column].totalRisk)
                {
                    graph[neighbour.row][neighbour.column].totalRisk = newTotalRisk;
                    graph[neighbour.row][neighbour.column].previous = current.location;
                    queue.push(graph[neighbour.row][neighbour.column]);
                }
            }
        }
    }


    if(save)
    {
        Location  walk={size-1,size-1};

        while(walk != Location{0,0})
        {
            graph[walk.row][walk.column].onPath=true;
            walk=graph[walk.row][walk.column].previous;
        }

        std::ofstream image{"cave.pgm"};

        image << "P2\n";
        image << size << ' ' << size << "\n";
        image << "255\n";

        auto maxRisk=0;

        for(auto const &row : graph)
        {
            for(auto const &node : row)
            {
                maxRisk=std::max(maxRisk, node.totalRisk);
            }
            image << "\n";
        }


        for(auto const &row : graph)
        {
            for(auto const &node : row)
            {
                int colour{};

                if(!node.visited)
                {
                    colour=0;
                }
                else if(node.onPath)
                {
                    colour=255;
                }
                else
                {
                    colour = maxRisk-node.totalRisk;

                    colour *= 255;
                    colour /= maxRisk;

                    colour = std::min(colour,255);
                }

                image << colour << ' ';
            }
            image << "\n";
        }



    }


    return graph[size-1][size-1].totalRisk;
}



