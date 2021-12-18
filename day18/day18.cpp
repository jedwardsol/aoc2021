#include <cassert>
#include <cstdint>

#include <string>
#include <cctype>

#include <iostream>
#include <sstream>
#include <format>


using namespace std::literals;
#include "include/thrower.h"

extern std::istringstream data;

using SnailNumber = std::string;

const auto digits="0123456789";


bool explode(SnailNumber &s)
{
    int depth{};

    for(int i=0;i<s.size();i++)
    {
        if(s[i]=='[')
        {
            depth++;
        }
        else if(s[i]==']')
        {
            depth--;
        }

        if(depth==5)
        {
             assert(i+4 < s.size());
             assert(std::isdigit(s[i+1]));
             assert(',' ==       s[i+2]);
             assert(std::isdigit(s[i+3]));
             assert(']' ==       s[i+4]);

             auto firstNumber  =i+1;
             auto secondNumber =i+3;
             
             auto previousNumber = s.find_last_of (digits,firstNumber-1);
             auto nextNumber     = s.find_first_of(digits,secondNumber+1);

            if(previousNumber!=s.npos)
            {
                s[previousNumber]+= s[firstNumber]-'0';

                assert(s[previousNumber] < '[');
            }

            if(nextNumber!=s.npos)
            {
                s[nextNumber]+= s[secondNumber]-'0';
                assert(s[nextNumber] < '[');
            }

            s.replace(i,5,"0");

            return true;
        }

    }

    return false;    
}

bool split(SnailNumber &s)
{
    for(int i=0;i<s.size();i++)
    {
        char c = s[i];

        if(    c >= ('0' + 10)
           &&  c  < '[' )
        {
            c -= '0';

            char first= c/2 + '0';

            char second = (c % 2 ) ? first +1 : first ;

            std::string pair = std::format("[{},{}]",first,second);

            s.replace(i,1,pair);


            return true;
        }
    }

    return false;    
}


void reduce(SnailNumber &s)
{
    bool didSomething{};

    do
    {
        didSomething = explode(s);

        if(!didSomething)
        {
            didSomething = split(s);
        }

    } while(didSomething);
}

SnailNumber add(SnailNumber const &lhs,SnailNumber const &rhs)
{
    auto sum = std::format("[{},{}]",lhs,rhs);
    reduce(sum);
    return sum;
}

SnailNumber sum(std::vector<SnailNumber> const &list)
{
    auto sum = list[0];

    for(int i=1;i<list.size();i++)
    {
        sum = add(sum,list[i]);
    }

    return sum;
}



void testExplode()
{
    auto test1   {SnailNumber{"[[[[[9,8],1],2],3],4]"}};
    auto result1 {SnailNumber{"[[[[0,9],2],3],4]"}};
    assert(explode(test1));
    assert(test1==result1);

    auto test2   {SnailNumber{"[7,[6,[5,[4,[3,2]]]]]"}};
    auto result2 {SnailNumber{"[7,[6,[5,[7,0]]]]"}};
    assert(explode(test2));
    assert(test2==result2);

    auto test3   {SnailNumber{"[[6,[5,[4,[3,2]]]],1]"}};
    auto result3 {SnailNumber{"[[6,[5,[7,0]]],3]"}};
    assert(explode(test3));
    assert(test3==result3);

    auto test4   {SnailNumber{"[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]"}};
    auto result4 {SnailNumber{"[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]"}};
    assert(explode(test4));
    assert(test4==result4);

    auto test5   {SnailNumber{"[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]"}};
    auto result5 {SnailNumber{"[[3,[2,[8,0]]],[9,[5,[7,0]]]]"}};
    assert(explode(test5));
    assert(test5==result5);
}


/*

    0    0
    1    1
    2    2
    3    3
    4    4
    5    5
    6    6
    7    7
    8    8
    9    9
   10    :
   11    ;
   12    <
   13    =
   14    >
   15    ?
   16    @
   17    A
   18    B
   19    C
   20    D
   21    E
   22    F
   23    G
   24    H
   25    I
   26    J
   27    K
   28    L
   29    M
   30    N

*/


void testSplit()
{
    auto test1   {SnailNumber{"[[[[0,7],4],[?,[0,=]]],[1,1]]"}};
    auto result1 {SnailNumber{"[[[[0,7],4],[[7,8],[0,=]]],[1,1]]"}};
    assert(split(test1));
    assert(test1==result1);

    auto test2   {SnailNumber{"[[[[0,7],4],[[7,8],[0,=]]],[1,1]]"}};
    auto result2 {SnailNumber{"[[[[0,7],4],[[7,8],[0,[6,7]]]],[1,1]]"}};
    assert(split(test2));
    assert(test2==result2);
}



void testReduce()
{
    auto test1   {SnailNumber{"[[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]"}};
    auto result1 {SnailNumber{"[[[[0,7],4],[[7,8],[6,0]]],[8,1]]"}};
    reduce(test1);
    assert(test1==result1);
}


void testAdd()
{
    { 
        auto lhs     {SnailNumber{"[[[[4,3],4],4],[7,[[8,4],9]]]"}};
        auto rhs     {SnailNumber{"[1,1]"}};
        auto result1 {SnailNumber{"[[[[0,7],4],[[7,8],[6,0]]],[8,1]]"}};
        auto sum     {add(lhs,rhs)};

        assert(sum==result1);
    }
}


void testSum()
{
    std::vector<SnailNumber> test1
    {
        "[1,1]",
        "[2,2]",
        "[3,3]",
        "[4,4]",
        "[5,5]",
        "[6,6]"
    };
    auto result1 {SnailNumber{"[[[[5,0],[7,4]],[5,5]],[6,6]]"}};

    auto sum1 = ::sum(test1);
    assert(sum1==result1);


}


int main()
try
{
    testExplode();
    testSplit();
    testReduce();
    testAdd();
    testSum();

}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}