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
#include "include/console.h"


extern std::istringstream testData;
extern std::istringstream realData;
auto &data=testData;

auto readinput()
{
    auto lines = std::vector<std::string>{};
    auto line  = ""s;

    while(std::getline(data,line))
    {
        lines.push_back(line);
        data.ignore();
    }

    return lines;
}


int main()
try
{
    auto lines=readinput();


    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}