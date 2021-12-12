#pragma once
#include <windows.h>
#include <format>
#include <iostream>

class Console
{
    HANDLE console;
    DWORD  originalMode{};

public:

    Console() : console{GetStdHandle(STD_OUTPUT_HANDLE )}
    {
        GetConsoleMode(console,&originalMode),
        SetConsoleMode(console, originalMode|ENABLE_VIRTUAL_TERMINAL_PROCESSING );
    }

    ~Console()
    {
        std::cout << "\x1b[!p";
        SetConsoleMode(console, originalMode);
    }


    void clear()
    {
        std::cout << "\x1b[2J";
    }

    void goTo(int row, int column)
    {
        std::cout << std::format("\x1b[{};{}H",row,column);
    }

    void grey(int percentBrightness)
    {
        auto rgb = 255*percentBrightness/100;

        std::cout <<  std::format("\x1b[38;2;{};{};{}m", rgb,rgb,rgb);
    }
};


