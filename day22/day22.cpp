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

#include <chrono>

namespace chr   = std::chrono;
using     Clock = chr::steady_clock;

using namespace std::literals;
#include "include/thrower.h"

#define DO_NAIVE true


struct Range
{
    int  start;
    int  end;            // inclusive

    bool little()
    {
        return    start > -50
               && end   < 50;
    };

    bool    contains(int i) const
    {
        return     i >= start
               &&  i <= end;
    }

};

struct Cuboid
{
    bool    on;
    Range   rx;
    Range   ry;
    Range   rz;


    bool    contains(int x,int y, int z) const
    {
        return     rx.contains(x)
               &&  ry.contains(y)
               &&  rz.contains(z);
    }
};


Range readRange(std::string_view   line)
{
    Range range;

    auto [next, error1] = std::from_chars(line.data(), line.data()+line.size(),range.start);
    assert(error1 == std::errc{});
                
    [[maybe_unused]] auto [_, error2] = std::from_chars(next+2, line.data()+line.size(),range.end);
    assert(error2 == std::errc{});

    assert(range.start < range.end);

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

    cuboid.rx = readRange(line.substr(x));
    cuboid.ry = readRange(line.substr(y));
    cuboid.rz = readRange(line.substr(z));

    assert (   cuboid.rx.little()   == cuboid.ry.little());
    assert (   cuboid.ry.little()   == cuboid.rz.little());

    return cuboid;
}


auto readCuboids()
{
    std::vector<Cuboid>     initialisationCuboids;
    std::vector<Cuboid>     rebootCuboids;

    std::ifstream           data{"data22.txt"};
//  std::ifstream           data{"test22.txt"};
    std::string             line;

    if(!data)
    {
        throw_runtime_error("no file");
    }

    while(std::getline(data,line))
    {
        auto cuboid  = readCuboid(line);

        if(cuboid.rx.little())
        {
            initialisationCuboids.push_back(cuboid);
        }
        else
        {
            rebootCuboids.push_back(cuboid);
        }
    }

    return std::make_pair(initialisationCuboids, rebootCuboids);
}



#if DO_NAIVE
std::array<std::array<std::array<bool,101>,101>,101>  cube{};

void part1_naive(std::vector<Cuboid> const &cuboids)
{
    for(auto cuboid : cuboids)
    {
        for(int x = cuboid.rx.start; x <= cuboid.rx.end; x++)
        for(int y = cuboid.ry.start; y <= cuboid.ry.end; y++)
        for(int z = cuboid.rz.start; z <= cuboid.rz.end; z++)
        {
            cube[x+50][y+50][z+50] = cuboid.on;
        }
    }

    int count{};
    for(auto & plane   : cube)
    for(auto & row     : plane)
    for(auto   element : row)
    {
        count += element;
    }

    std::cout << "Part 1 : " << count << "\n";

}

// right answer is 564654

#endif DO_NAIVE
               

int64_t count(std::vector<Cuboid> const &cuboids)
{
    std::vector<int>        xs;
    std::vector<int>        ys;
    std::vector<int>        zs;

    for(auto const &cuboid : cuboids)
    {
        xs.push_back(cuboid.rx.start);
        xs.push_back(cuboid.rx.end+1);
        ys.push_back(cuboid.ry.start);
        ys.push_back(cuboid.ry.end+1);
        zs.push_back(cuboid.rz.start);
        zs.push_back(cuboid.rz.end+1);
    }

    auto uniq = [](auto &range)
    {
        std::ranges::sort(range);

        auto [eraseStart, eraseEnd] = std::ranges::unique(range);
        range.erase(eraseStart, eraseEnd);
    };

    uniq(xs);
    uniq(ys);
    uniq(zs);


    // space is partitioned into a lot of smaller cuboids.
    // each of which is either fully on or fully off

    int64_t count{};

    for(int xi=0; xi < xs.size() - 1 ; xi++)
    {
        for(int yi=0; yi < ys.size() - 1 ; yi++)
        {
            for(int zi=0; zi < zs.size() - 1 ; zi++)
            {
                bool    on=false;

                for(auto &cuboid : cuboids)
                {
                    if(cuboid.contains(xs[xi],ys[yi],zs[zi]))
                    {
                        on = cuboid.on;
                    }
                }


                if(on)
                {
                    count +=   1ll
                             * (xs[xi+1] - xs[xi] )
                             * (ys[yi+1] - ys[yi] )
                             * (zs[zi+1] - zs[zi] );
                }
            }
        }
    }


    return count;
}


int main()
try
{
    auto [initialisationCuboids,rebootCuboids] = readCuboids();


#if DO_NAIVE
    part1_naive(initialisationCuboids);
#endif DO_NAIVE

    auto initialisationCount = count(initialisationCuboids);
 
    auto start = Clock::now();
    auto rebootCount         = count(rebootCuboids);
    auto end   = Clock::now();

    std::cout << "Part 1 : " << initialisationCount << "\n";
    std::cout << "Part 2 : " << initialisationCount + rebootCount << "\n";


    auto duration = chr::duration_cast<chr::seconds>(end-start);

    std::cout << std::format("Time : {}\n",duration);

    // 10 minutes!
    // half a billion cuboids.
    // parallelisable

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}