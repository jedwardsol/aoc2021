#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <map>
#include <string>

#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"

extern std::istringstream &data;


class Cave
{
public:

    Cave()
    {
        throw_runtime_error("Default constructed cave");
    }

    Cave(std::string const &name)
    {
        if(name=="end")
        {
            type=Type::end;
        }
        else if(std::islower(name[0]))
        {
            type=Type::little;
        }
        else
        {
            type=Type::big;
        }
    }

    static void join(Cave &cave1, Cave &cave2)
    {
        assert( std::ranges::find(cave1.neighbours,&cave2) == cave1.neighbours.end());
        assert( std::ranges::find(cave2.neighbours,&cave1) == cave2.neighbours.end());

        cave1.neighbours.push_back(&cave2);
        cave2.neighbours.push_back(&cave1);
    }


    void visit1()
    {
        visitedCount++;

        if(type==Type::end)
        {
            return;
        }
        else if(type==Type::little)
        {
            visited=true;
        }

        for(auto neighbour:neighbours)
        {
            if(!neighbour->visited)
            {
                neighbour->visit1();
            }
        }

        visited=false;
    }

    auto getCount()
    {
        return visitedCount;
    }


private:

    enum class Type
    {
        end,
        big,
        little
    };

    Type                type;
    bool                visited{};
    int                 visitedCount{};
    std::vector<Cave*>  neighbours;

};



auto readCaves()
{
    std::map<std::string,Cave>      caves;
    std::string                     line;


    while(std::getline(data,line))
    {
        if(!line.empty())
        {
            auto dash=line.find('-');
            assert(dash!=line.npos);

            auto cave1=line.substr(0,dash);
            auto cave2=line.substr(dash+1);

            caves.insert(std::make_pair(cave1, Cave{cave1}));
            caves.insert(std::make_pair(cave2, Cave{cave2}));

            Cave::join(caves[cave1],caves[cave2]);                

        }
    }

    return caves;
}

int main()
try
{
        
    auto      part1{readCaves()};
    auto      part2{part1};

    part1["start"].visit1();

    std::cout << "Part 1 : " << part1["end"].getCount() << "\n";
    std::cout << "Part 2 : " << part2["end"].getCount() << "\n";


    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}