#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <variant>
#include <map>
#include <queue>

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


    int operator()(int v)
    {
        return v;
    }
    
    int operator()(Instruction::Register r)
    {
        return registers[r];
    }


    void run(    std::queue<int>         inputs)
    {
        for(auto &instruction : program)
        {
            debug(instruction.instruction);
        
            switch(instruction.opcode)            
            {
            case Instruction::OpCode::inp:
                registers[instruction.destination] = inputs.front();
                inputs.pop();
                break;

            case Instruction::OpCode::add:
                registers[instruction.destination] =    registers[instruction.destination]
                                                     +  std::visit(*this, instruction.source);
                break;

            case Instruction::OpCode::mul:
                registers[instruction.destination] =    registers[instruction.destination]
                                                     *  std::visit(*this, instruction.source);
                break;

            case Instruction::OpCode::div:
                registers[instruction.destination] =    registers[instruction.destination]
                                                     /  std::visit(*this, instruction.source);
                break;

            case Instruction::OpCode::mod:
                registers[instruction.destination] =    registers[instruction.destination]
                                                     %  std::visit(*this, instruction.source);
                break;

            case Instruction::OpCode::eql:
                registers[instruction.destination] =    registers[instruction.destination]
                                                     == std::visit(*this, instruction.source);
                break;

            default:
                throw_runtime_error("bad instruction");
            }
        }

        debug("end");

    }

    void debug(std::string const &nextInstruction)
    {
        if(nextInstruction.starts_with("inp"))
        {
            std::cout << "\n\n";
        }

        std::cout << std::format("w={:10}    x={:10}    y={:10}    z={:10}    \n",registers[0],
                                                                                  registers[1],
                                                                                  registers[2],
                                                                                  registers[3]);

        std::cout << std::format("{:80} next : {}\n","",nextInstruction);

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

    alu.run(std::queue<int>{{1,2,3,4,5,6,7,1,2,3,4,5,6,7}});


    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}