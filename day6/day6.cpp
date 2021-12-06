#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <format>
#include <deque>

using namespace std::literals;
#include "include/thrower.h"





int main()
try
{
    auto data = std::ifstream{"data.txt"};

    if(!data)
    {
        throw_system_error("data.txt");
    }


    std::deque<char>             fish;
    int                         age;

    while(data >> age)
    {
        fish.push_back(age);
        data.ignore();
    }

    for(int i=0;i<80;i++)
    {
        for(auto &age : fish)
        {
            switch(age)
            {
            case 0:
                age=6;
                fish.push_back(8);
                break;

            default:
                age--;
            }
        }
    }

    std::cout << fish.size();

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}