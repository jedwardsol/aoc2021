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


constexpr int comma=-9;
constexpr int open =-1;
constexpr int close=-2;

using SnailNumber = std::vector<int>;


bool isComma(int c)
{
    return c == comma;
}

bool isOpen(int c)
{
    return c == open;
}

bool isClose(int c)
{
    return c == close;
}

bool isDigit(int c)
{
    return c >= 0;
}



SnailNumber from_string(std::string const &s)
{
    SnailNumber n;

    for(auto c: s)
    {
        if(c == '[')
        {
            n.push_back(open);
        }
        else if(c == ']')
        {
            n.push_back(close);
        }
        else if(c == ',')
        {
            n.push_back(comma);
        }
        else
        {
            n.push_back(c-'0');
        }
    }

    return n;
}


std::string to_string(SnailNumber const &n)
{
    std::string s;

    for(auto i: n)
    {
        if(i == open)
        {
            s.push_back('[');
        }
        else if(i == close)
        {
            s.push_back(']');
        }
        else if(i == comma)
        {
            s.push_back(',');
        }
        else
        {
            s.push_back(i+'0');
        }
    }

    return s;
}


int findPreviousDigit(SnailNumber const &s, int startAt)
{
    for(int i=startAt-1;i >=0; i--)
    {
        if( isDigit(s[i]))
        {
            return i;
        }
    }

    return -1;
}

int findNextDigit(SnailNumber const &s, int startAt)
{
    for(int i=startAt+1;i < s.size(); i++)
    {
        if( isDigit(s[i]))
        {
            return i;
        }
    }

    return -1;
}




bool explode(SnailNumber &s)
{
    int depth{};

    for(int i=0;i<s.size();i++)
    {
        if(s[i]==open)
        {
            depth++;
        }
        else if(s[i]==close)
        {
            depth--;
        }



        if(depth==5)
        {
             assert(i+4 < s.size());
             assert(isDigit(s[i+1]));
             assert(isComma(s[i+2]));
             assert(isDigit(s[i+3]));
             assert(isClose(s[i+4]));

             auto firstNumber       =i+1;
             auto secondNumber      =i+3;
             
             auto previousNumber    = findPreviousDigit(s,firstNumber);
             auto nextNumber        = findNextDigit    (s,secondNumber);

            if(previousNumber!= -1)
            {
                s[previousNumber]+= s[firstNumber];
            }

            if(nextNumber!= -1)
            {
                s[nextNumber]+= s[secondNumber];
            }

            s.erase(s.begin()+i, s.begin()+i+5);
            s.insert(s.begin()+i,0);

            return true;
        }

    }

    return false;    
}

bool split(SnailNumber &s)
{
    for(int i=0;i<s.size();i++)
    {
        auto digit = s[i];

        if(digit >= 10)
        {
            auto first  = digit/2 ;
            auto second = (digit % 2 ) ? first +1 : first ;

            SnailNumber replacement { open, first, comma, second, close };

            s.erase(s.begin()+i);

            s.insert(s.begin()+i, replacement.begin(), replacement.end());

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
        auto previous = to_string(s);

        didSomething = explode(s);

        if(!didSomething)
        {
            didSomething = split(s);
        }

        auto result = to_string(s);


    } while(didSomething);
}

SnailNumber add(SnailNumber const &lhs,SnailNumber const &rhs)
{
    SnailNumber sum;

    sum.push_back(open);
    sum.insert(sum.end(),lhs.begin(), lhs.end());
    sum.push_back(comma);
    sum.insert(sum.end(),rhs.begin(), rhs.end());
    sum.push_back(close);

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
    auto test1    = from_string("[[[[[9,8],1],2],3],4]");
    auto result1  = from_string("[[[[0,9],2],3],4]");
    assert(explode(test1));
    assert(test1==result1);

    auto test2   = from_string("[7,[6,[5,[4,[3,2]]]]]");
    auto result2 = from_string("[7,[6,[5,[7,0]]]]");
    assert(explode(test2));
    assert(test2==result2);

    auto test3   = from_string("[[6,[5,[4,[3,2]]]],1]");
    auto result3 = from_string("[[6,[5,[7,0]]],3]");
    assert(explode(test3));
    assert(test3==result3);

    auto test4   = from_string("[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]");
    auto result4 = from_string("[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]");
    assert(explode(test4));
    assert(test4==result4);

    auto test5   = from_string("[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]");
    auto result5 = from_string("[[3,[2,[8,0]]],[9,[5,[7,0]]]]");
    assert(explode(test5));
    assert(test5==result5);
}


void testSplit()
{
    auto test0   = from_string("[0,:]");
    auto result0 = from_string("[0,[5,5]]");
    assert(split(test0));
    auto string0=to_string(test0);
    assert(test0==result0);


    auto test1   = from_string("[[[[0,7],4],[?,[0,=]]],[1,1]]");
    auto result1 = from_string("[[[[0,7],4],[[7,8],[0,=]]],[1,1]]");

    assert(split(test1));

    auto string1=to_string(test1);

    assert(test1==result1);

    auto test2   = from_string("[[[[0,7],4],[[7,8],[0,=]]],[1,1]]");
    auto result2 = from_string("[[[[0,7],4],[[7,8],[0,[6,7]]]],[1,1]]");
    assert(split(test2));
    assert(test2==result2);
}




void testReduce()
{
    auto test1   = from_string("[[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]");
    auto result1 = from_string("[[[[0,7],4],[[7,8],[6,0]]],[8,1]]");
    reduce(test1);
    assert(test1==result1);
}

void testAdd()
{
    {
        auto rhs     = from_string("[1,1]");
        auto expected= from_string("[[1,1],[1,1]]");

        auto sum     {add(rhs,rhs)};
        
        auto string = to_string(sum);

        assert(sum==expected);
    }


    {
        auto lhs     = from_string("[[[[4,3],4],4],[7,[[8,4],9]]]");
        auto rhs     = from_string("[1,1]");
        auto expected= from_string("[[[[0,7],4],[[7,8],[6,0]]],[8,1]]");

        auto sum     {add(lhs,rhs)};
        assert(sum==expected);
    }


    {
        auto lhs      = from_string("[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]");
        auto rhs      = from_string("[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]");
        auto expected = from_string("[[[[4,0],[5,4]],[[7,7],[6,0]]],[[8,[7,7]],[[7,9],[5,0]]]]");
        auto sum     {add(lhs,rhs)};
        assert(sum==expected);
    }


}


void testSum()
{
    std::vector<SnailNumber> test1
    {
        from_string("[1,1]"),
        from_string("[2,2]"),
        from_string("[3,3]"),
        from_string("[4,4]"),
        from_string("[5,5]"),
        from_string("[6,6]")
    };
    auto result1 = from_string("[[[[5,0],[7,4]],[5,5]],[6,6]]");

    auto sum1 = ::sum(test1);
    assert(sum1==result1);


    std::vector<SnailNumber> test2
    {
        from_string("[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]"),
        from_string("[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]"),
        from_string("[[2,[[0,8],[3,4]]],[[[6,7],1],[7,[1,6]]]]"),
        from_string("[[[[2,4],7],[6,[0,5]]],[[[6,8],[2,8]],[[2,1],[4,5]]]]"),
        from_string("[7,[5,[[3,8],[1,4]]]]"),
        from_string("[[2,[2,2]],[8,[8,1]]]"),
        from_string("[2,9]"),
        from_string("[1,[[[9,3],9],[[9,0],[0,7]]]]"),
        from_string("[[[5,[7,4]],7],1]"),
        from_string("[[[[4,2],2],6],[8,7]]")
    };
    auto result2 = from_string("[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]");

    auto sum2 = ::sum(test2);
    assert(sum2==result2);



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