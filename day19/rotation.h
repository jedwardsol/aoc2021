#pragma once
#include <array>

using Location    = std::array<int,3>;
using Translation = std::array<int,3>;
using Rotation    = std::array<std::array<int,3>,3>;



constexpr Translation operator-(Location const &lhs, Location const &rhs)
{
    return {{  lhs[0]-rhs[0],
               lhs[1]-rhs[1],
               lhs[2]-rhs[2]
           }};
}

constexpr Location operator+(Location const &lhs, Translation const &rhs)
{
    return {{  lhs[0]+rhs[0],
               lhs[1]+rhs[1],
               lhs[2]+rhs[2]
           }};
}


constexpr Location      testStart           {{ 1,2,3 }};
constexpr Location      testEnd             {{ 5,7,9 }};
constexpr Translation   testTranslation     { testStart - testEnd};

constexpr Location      testRelocated       { testEnd+testTranslation };

static_assert(testTranslation == Translation{{-4,-5,-6}});
static_assert(testRelocated == testStart);



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
