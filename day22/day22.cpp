#include <cassert>
#include <cstdint>

#include <array>
#include <vector>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"


struct Range
{
    int start;
    int end;            // inclusive
};

struct Cuboid
{
    bool    on;
    Range   x;
    Range   y;
    Range   z;
};


Range readRange(std::string_view   line)
{
    Range range;

    auto [next, error1] = std::from_chars(line.data(), line.data()+line.size(),range.start);
    assert(error1 == std::errc{});
                
    auto [_, error2] = std::from_chars(next+2, line.data()+line.size(),range.end);
    assert(error2 == std::errc{});

    return range;
}

Cuboid readCuboid(std::string_view   line)
{
    Cuboid  cuboid{};

    //on x=-31..18,y=-8..36,z=-8..37

    cuboid.on = line.starts_with("on");

    auto x = line.find("x=");
    auto y = line.find("y=");
    auto z = line.find("z=");

    assert(x!=line.npos);
    assert(y!=line.npos);
    assert(y!=line.npos);

    x+=2;
    y+=2;
    z+=2;

    cuboid.x = readRange(line.substr(x));
    cuboid.y = readRange(line.substr(y));
    cuboid.z = readRange(line.substr(z));

    return cuboid;
}


auto readCuboids()
{
    std::vector<Cuboid>     cuboids;

    std::ifstream           data{"data22.txt"};
    std::string             line;

    if(!data)
    {
        throw_runtime_error("no file");
    }

    while(std::getline(data,line))
    {
        cuboids.push_back(readCuboid(line));
    }

    return cuboids;
}

bool  grid[101][101][101]{};

int main()
try
{
    auto cuboids = readCuboids();

    for(auto &cuboid : cuboids)
    {
        if(   cuboid.x.start < -50    
           || cuboid.x.start < -50    
           || cuboid.x.start < -50    
           || cuboid.x.start < -50    
           || cuboid.x.start < -50    
           || cuboid.x.start < -50    


    }

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}