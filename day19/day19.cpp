#include <cassert>
#include <cstdint>

#include <array>
#include <vector>


#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"

extern std::istringstream realData;
extern std::istringstream testData;


/*
--- scanner 0 ---
-676,-433,-499
496,-527,-542
*/

using Beacon = std::array<int,3>;

struct Scanner
{
    std::vector<Beacon>     beacons;

};

auto readData(std::istringstream &data)
{
    std::vector<Scanner>    scanners;
    std::string             line;


    while(std::getline(data,line))
    {
        if(line.empty())
        {
            continue;
        }

        if(line.starts_with("--- scanner "s))
        {
            scanners.emplace_back();
        }
        else
        {
            std::istringstream  coords{line};
            Beacon              beacon{};
            char                comma;

            coords >> beacon[0] >> comma >> beacon[1] >> comma >> beacon[2];

            assert(coords);

            scanners.back().beacons.push_back(beacon);
        }
    }

    return scanners;
}



int main()
try
{

    auto scanners=readData(testData);


    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}