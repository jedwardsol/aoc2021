#pragma once
#include <array>

using Location = std::array<int,3>;
using Rotation = std::array<std::array<int,3>,3>;

constexpr Rotation operator*(Rotation const &lhs,Rotation const &rhs)
{
    Rotation result{};

    for(int row=0;row<3;row++)
    {
        for(int column=0;column<3;column++)
        {
            int answer{};

            for(int i=0;i<3;i++)
            {
                answer +=  lhs[row][i] * rhs[i][column];
            }

            result[row][column]=answer;
        }
    }

    return result;
}

constexpr Location operator*(Rotation const &lhs,Location const &rhs)
{
    Location result{};

    for(int row=0;row<3;row++)
    {
        int answer{};

        for(int i=0;i<3;i++)
        {
            answer +=  lhs[row][i] * rhs[i];
        }

        result[row]=answer;

    }

    return result;
}







////




constexpr Rotation RtestLHS
{{
    { 1, 2, 3},
    {-4,-5,-6},
    { 2, 3, 5},
}};

constexpr Rotation RtestRHS
{{
    { 3, 1, 4},
    { 1, 5, 9},
    { 2, 6, 5},
}};

constexpr Location LtestRHS
{{ 
    2, 
    7, 
    1
}};


constexpr Rotation RtestExpected
{{
    { 11, 29, 37},
    {-29,-65,-91},
    { 19, 47, 60},
}};

constexpr Location LtestExpected
{{ 
     19, 
    -49, 
     30
}};


static_assert(RtestLHS * RtestRHS == RtestExpected);
static_assert(RtestLHS * LtestRHS == LtestExpected);
