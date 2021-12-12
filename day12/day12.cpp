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

extern std::string &data;


class Cave
{
public:

    Cave()
    {
        throw_runtime_error("Default constructed cave");
    }

    Cave(std::string const &name) : name{name}
    {
        if(name=="end")
        {
            type=Type::end;
        }
        else if(name=="start")
        {
            type=Type::start;
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

    auto getCount()
    {
        return visitedCount;
    }

    void visit1()
    {
        visitedCount++;

        if(type==Type::end)
        {
            return;
        }

        for(auto neighbour:neighbours)
        {
            if(neighbour->type == Type::start)
            {
                continue;
            }

            if(    neighbour->type       != Type::little
               ||  neighbour->getCount() == 0)
            {
                neighbour->visit1();
            }
        }

        visitedCount--;
    }


    bool noneHaveBeenVistedTwiceYet(std::vector<Cave*> &path)
    {
        for(auto cave : path)
        {
            if(    cave->type       == Type::little
               &&  cave->getCount() == 2)
            {
                return false;
            }
        }

        return true;
    }


    void visit2()
    {
        std::vector<Cave*> path;

        visit2(path);
    }

    void visit2(std::vector<Cave*> &path)
    {
        visitedCount++;
        path.push_back(this);


        if(type==Type::end)
        {
/*
            std::string comma;        

            for(auto cave : path)
            {
                std::cout << comma << cave->name;
                comma=',';
            }
            std::cout << '\n';
*/
            path.pop_back();
            return;
        }

        for(auto neighbour:neighbours)
        {
            if(neighbour->type == Type::start)
            {
                continue;
            }

            if(    neighbour->type         != Type::little
               ||  neighbour->getCount()   == 0
               ||  noneHaveBeenVistedTwiceYet(path))
            {
                neighbour->visit2(path);
            }
        }

        path.pop_back();
        visitedCount--;
    }



private:

    enum class Type
    {
        start,
        end,
        big,
        little
    };

    std::string         name;
    Type                type;

    int                 visitedCount{};
    std::vector<Cave*>  neighbours;

};



auto readCaves()
{
    std::istringstream              stream{data};
    
    std::map<std::string,Cave>      caves;
    std::string                     line;
    

    
    while(std::getline(stream,line))
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
    auto      part2{readCaves()};

    part1["start"].visit1();
    part2["start"].visit2();

    std::cout << "Part 1 : " << part1["end"].getCount() << "\n";
    std::cout << "Part 2 : " << part2["end"].getCount() << "\n";

    assert(part1["end"].getCount() == 4411);

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}