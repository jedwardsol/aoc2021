#include <tuple>
#include <array>
#include <vector>
#include <iostream>
#include <format>
#include <string>
#include <sstream>
#include <fstream>
#include <exception>
#include <stdexcept>

#include "include/thrower.h"



auto readCalledNumbers(std::ifstream &data)
{
    std::string line;
    std::getline(data,line);
    std::istringstream numbers{line};

    std::vector<int>  calledNumbers;

    int calledNumber;

    while(numbers >> calledNumber)
    {
        calledNumbers.push_back(calledNumber);

        numbers.ignore();   // comma
    }


    if(calledNumbers.empty())
    {
        throw_runtime_error("No numbers");
    }

    return calledNumbers;
}



using Square = std::pair<int,bool>;
struct Board 
{
    std::array<std::array<Square,5>,5>  board;
    bool                                won;
};




bool readBoard(std::ifstream &data, Board &board)
{
    memset(&board,0,sizeof(board));

    std::string blankLine;
    std::getline(data,blankLine);


    std::array<std::string,5>  lines;


    if(!blankLine.empty())
    {
        throw_runtime_error("non-blank line");
    }

    for(auto &line : lines)
    {
        if(!std::getline(data,line))
        {
            return false;
        }

        if(line.empty())
        {
            throw_runtime_error("blank line");
        }
    }


    for(int line = 0; line < 5; line++)
    {
        std::istringstream  numbers{lines[line]};

        for(auto &square : board.board[line])
        {
            numbers >> square.first;
        }
    }


    return true;
}


auto readBoards(std::ifstream  &data)
{
    std::vector<Board>  boards;
    Board board;

    while(readBoard(data,board))
    {
        boards.push_back(board);
    }

    if(boards.empty())
    {
        throw_runtime_error("No numbers");
    }

    return boards;
}


bool isBoardComplete(Board const &board)
{
    for(auto line=0;line<5;line++)
    {
        bool lineComplete=true;

        for(auto column=0;column<5;column++)
        {
            if(!board.board[line][column].second)
            {
                lineComplete=false;
            }
        }

        if(lineComplete)
        {
            return true;
        }
    }


    for(auto column=0;column<5;column++)
    {
        bool columnComplete=true;

        for(auto line=0;line<5;line++)
        {
            if(!board.board[line][column].second)
            {
                columnComplete=false;
            }
        }

        if(columnComplete)
        {
            return true;
        }
    }

    return false;
}


int boardScore(Board const &board, int calledNumber)
{
    int sum{};

    for(auto &line : board.board)
    {
        for(auto &square : line)
        {
            if(!square.second)
            {
                sum+=square.first;
            }
        }
    }

    return sum*calledNumber;
}


int part1(std::vector<Board> &boards, std::vector<int> const &calledNumbers)
{
    for(auto calledNumber : calledNumbers)
    {
        for(auto &board : boards)
        {
            for(auto &line : board.board)
            {
                for(auto &square : line)
                {
                    if(square.first == calledNumber)
                    {
                        square.second = true;
                
                        if(isBoardComplete(board))
                        {
                            board.won=true;
                            return boardScore(board,calledNumber);
                        }
                    }
                }
            }
        }
    }

    throw_runtime_error("part1 failed\n");

}


void playUntilEnd(std::vector<Board> &boards, std::vector<int> const &calledNumbers)
{
    for(auto calledNumber : calledNumbers)
    {
        for(auto &board : boards)
        {
            if(!board.won)
            {
                for(auto &line : board.board)
                {
                    for(auto &square : line)
                    {
                        if(square.first == calledNumber)
                        {
                            square.second = true;
                
                            if(isBoardComplete(board))
                            {
                                board.won=true;
                                std::cout << "Board wins with a score of << " << boardScore(board,calledNumber) << "\n";
                            }
                        }
                    }
                }
            }
        }
    }
}



int main()
try
{
    auto  data {std::ifstream{"data.txt"}};

    if(!data)
    {
        throw_runtime_error("can't open file");
    }

    auto calledNumbers = readCalledNumbers(data);    
    auto boards        = readBoards(data);

//  auto part1Answer   = part1(boards,calledNumbers);
//  std::cout << "Part1 = " << part1Answer << "\n";

    playUntilEnd(boards,calledNumbers);



}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}