#include <iostream>
#include <format>
#include <string>
#include <sstream>
#include <fstream>
#include <exception>
#include <stdexcept>

#include "include/thrower.h"

int main()
try
{
    auto  data {std::ifstream{"data.txt"}};

    if(!data)
    {
        throw_runtime_error("can't open file");
    }

    std::string  line;
    int          position{};
    int          depth{};
    int          aim{};
    
    while(std::getline(data,line))
    {
        std::istringstream  command{line};
        std::string         direction;
        int                 distance{};

        if(!(command >> direction >> distance))
        {
            throw_runtime_error("can't parse line " + line);
        }

        if(direction=="up")
        {
            aim     -=distance;
        }
        else if(direction=="down")
        {
            aim     +=distance;
        }
        else if(direction=="forward")  
        {
            position+=distance;
            depth   +=aim*distance;           
        }
        else 
        {
            throw_runtime_error("Invalid command " + direction);
        }

        if(depth < 0)
        {
            throw_runtime_error("Depth is " + depth);
        }

    }

    std::cout << std::format("{}*{}={}\n",position ,depth, position*depth);
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}