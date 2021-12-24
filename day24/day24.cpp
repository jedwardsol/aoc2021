#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <variant>
#include <map>

#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"

extern std::istringstream data;


struct Instruction
{
    enum class OpCode
    {
        invalid,
        inp,
        add,
        mul,
        div,
        mod,
        eql
    };

    enum Register
    {
        w,x,y,z
    };


    OpCode                          opcode;
    Register                        destination;
    std::variant<Register,int>      source;
    std::string                     instruction;

    Instruction(std::string const &line) : instruction{line}
    {
        static  std::map<std::string,OpCode> assembler
        {
            {"inp", OpCode::inp},
            {"add", OpCode::add},
            {"mul", OpCode::mul},
            {"div", OpCode::div},
            {"mod", OpCode::mod},
            {"eql", OpCode::eql},
        };

        opcode      = assembler[line.substr(0,3)];
        destination = static_cast<Register>(line[4]-'w');

        assert(opcode != OpCode::invalid);
        assert(0 <= destination && destination <= 3);
 
        if(opcode != OpCode::inp)
        {
            auto token = line.substr(6);

            if(    token[0] >= 'w'
               &&  token[0] <= 'z')
            {
                source = static_cast<Register>(token[0]-'w');
            }
            else
            {
                source = stoi(token);
            }
        }
    }
};


using Program = std::vector<Instruction>;

struct ALU
{
    Program                 program;
    std::array<int,4>       registers{};       // w,x,y,z

    ALU(Program program) : program{std::move(program)}
    {}

    void run()
    {

        for(auto &instruction : program)
        {
            debug(instruction.instruction);
        
            switch(instruction.opcode)            
            {
            case Instruction::OpCode::inp:

            }


        }

    }

    void debug(std::string const &nextInstruction)
    {


    }



};


int main()
try
{
    std::string     line;
    Program         program;

    while(std::getline(data,line))
    {
        if(!line.empty())
        {
            program.emplace_back(line);
        }
    }

    ALU alu{program};

    alu.run();


    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}