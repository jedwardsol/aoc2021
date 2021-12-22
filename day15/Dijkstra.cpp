#include <cassert>
#include <cstdint>

#include <vector>
#include <queue>
#include <fstream>

#include <limits>

#include <Windows.h>

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

using Graph         = std::vector<std::vector<Node>>;
using PriorityQueue = std::priority_queue<Node,std::vector<Node>,NodeCompare>;


void writePGM(Graph const &graph);
void writeBMP(Graph const &graph);


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
    Graph           graph(size, std::vector(size,Node{}));


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


    Location  walk={size-1,size-1};

    while(walk != Location{0,0})
    {
        graph[walk.row][walk.column].onPath=true;
        walk=graph[walk.row][walk.column].previous;
    }

    if(save)
    {
        //writePGM(graph);
        writeBMP(graph);
    }


    return graph[size-1][size-1].totalRisk;
}

/*

void writePGM(Graph const &graph)
{
    auto const      size=static_cast<int>(graph.size());


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
*/


void writeBMP(Graph const &graph)
{
    auto const      size=static_cast<int>(graph.size());

    std::ofstream image{"cave.bmp", std::ios::binary|std::ios::out};


    BITMAPFILEHEADER    fileHeader
    {
        'MB',
        sizeof(fileHeader) + sizeof(BITMAPINFOHEADER ) + size*size*3,
        0,0,
        sizeof(fileHeader) + sizeof(BITMAPINFOHEADER ) 
    };

    BITMAPINFOHEADER      infoHeader
    {
        sizeof(BITMAPINFOHEADER ),
        size,-size,
        1,
        24,
        BI_RGB,
        0,
        0,0,
        0,0,
    };


    image.write(reinterpret_cast<const char*>(&fileHeader),sizeof(fileHeader));
    image.write(reinterpret_cast<const char*>(&infoHeader),sizeof(infoHeader));

    auto maxRisk=graph[size-1][size-1].totalRisk;


    for(auto const &row : graph)
    {
        for(auto const &node : row)
        {
            struct Pixel
            { uint8_t b,g,r;
            } pixel;

            if(!node.visited)
            {
                pixel={0,0,255};
            }
            else if(node.onPath)
            {
                pixel={100,255,100};
            }
            else if(node.totalRisk > maxRisk)
            {
                pixel={128,128,255};
            }
            else
            {
                pixel.r = pixel.g = pixel.b = (maxRisk-node.totalRisk) * 255/maxRisk;
            }

            image.write(reinterpret_cast<const char*>(&pixel),sizeof(pixel));
        }
    }
}