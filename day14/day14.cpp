#define NOMINMAX
#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <tuple>
#include <map>

#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"

extern std::istringstream test1Data;
extern std::istringstream test2Data;
extern std::istringstream realData;


using Pair=std::pair<char,char>;
using Rules=std::map<Pair,char>;


struct Polymer
{
    std::string                 original;     // only really need to remember 1st and last since they're all that's needed to count accurately
    std::map<Pair,int64_t>      pairs;

    auto getCounts() const
    {
        std::map<char,int64_t>      counts;

        for(auto const &[pair,count] : pairs)
        {
            counts[pair.first] +=count;
            counts[pair.second]+=count;
        }

        for(auto &[c,count] : counts)
        {
            if(   c == original.front()
               && c == original.back())
            {
                // double counted,  except twice;
                count = 2+(count-2)/2;
            }
            else if(   c == original.front()
                    || c == original.back())
            {
                // double counted,  except once;
                count = 1+(count-1)/2;
            }
            else
            {
                // double counted

                count /= 2;
            }
        }

        return counts;
    }
};



Polymer polymerise(Polymer const &polymer, Rules &rules)
{
    Polymer result{polymer.original};

    for(auto [pair,count] : polymer.pairs)
    {
        auto toInsert = rules[pair];

        assert(std::isupper(toInsert));

        result.pairs[{ pair.first, toInsert}]   +=count;
        result.pairs[{ toInsert,   pair.second}]+=count;
    }

    return result;
}


auto parsePolymer(std::string const &line)
{
    Polymer polymer{line};

    for(int i=0;i<line.size()-1;i++)
    {
        polymer.pairs[{line[i],line[i+1]}]++;
    }

    return polymer;
}

auto parseInput(std::istringstream &data)
{
    Polymer     polymer;
    Rules       rules;

    std::string line;

    while(std::getline(data,line))
    {
        if(line.empty())
        {
            continue;
        }

        auto arrow=line.find("->"s);

        if(arrow==line.npos)
        {
            polymer = parsePolymer(line);
        }
        else
        {
            // CH -> B
            assert(std::isupper(line[0]));
            assert(std::isupper(line[1]));
            assert(std::isupper(line[arrow+3]));

            rules[{line[0], line[1]}]=line[arrow+3];
        }
    }

    return std::make_pair(polymer,rules);
}



void test1()
{
    {
        auto [polymer1, rules1] = parseInput(test1Data);
        auto counts1            = polymer1.getCounts();

        assert(rules1.size()                           == 16);

        assert(polymer1.original                       == "NNCB"s);
        assert(polymer1.pairs.size()                   == 3);
        assert(polymer1.pairs[std::make_pair('N','N')] == 1);
        assert(polymer1.pairs[std::make_pair('N','C')] == 1);
        assert(polymer1.pairs[std::make_pair('C','B')] == 1);

        assert(counts1.size()                          == 3);
        assert(counts1['N']                            == 2);
        assert(counts1['C']                            == 1);
        assert(counts1['B']                            == 1);

                                                                   //           _ _ _ _
        auto result1            = polymerise(polymer1,rules1);     //  NNCB  -> NCNBCHB
        auto counts1_1          = result1.getCounts();

        assert(result1.original                        == "NNCB"s);
        assert(result1.pairs.size()                    == 6);
        assert(result1.pairs[std::make_pair('N','C')]  == 1);
        assert(result1.pairs[std::make_pair('C','N')]  == 1);
        assert(result1.pairs[std::make_pair('N','B')]  == 1);
        assert(result1.pairs[std::make_pair('B','C')]  == 1);
        assert(result1.pairs[std::make_pair('C','H')]  == 1);
        assert(result1.pairs[std::make_pair('H','B')]  == 1);

        assert(counts1_1.size()                        == 4);
        assert(counts1_1['N']                          == 2);
        assert(counts1_1['C']                          == 2);
        assert(counts1_1['B']                          == 2);
        assert(counts1_1['H']                          == 1);
    }

    {
        auto [polymer2, rules2] = parseInput(test2Data);
        auto counts2            = polymer2.getCounts();

        assert(rules2.size()                           == 16);

        assert(polymer2.original                       == "NNCBN"s);
        assert(polymer2.pairs.size()                   == 4);
        assert(polymer2.pairs[std::make_pair('N','N')] == 1);
        assert(polymer2.pairs[std::make_pair('N','C')] == 1);
        assert(polymer2.pairs[std::make_pair('C','B')] == 1);
        assert(polymer2.pairs[std::make_pair('B','N')] == 1);

        assert(counts2.size()                          == 3);
        assert(counts2['N']                            == 3);
        assert(counts2['C']                            == 1);
        assert(counts2['B']                            == 1);

                                                                   //           _ _ _ _ _ 
        auto result2            = polymerise(polymer2,rules2);     //  NNCBN -> NCNBCHBBN             
        auto counts2_1          = result2.getCounts();

        assert(result2.original                        == "NNCBN"s);
        assert(result2.pairs.size()                    == 8);

        assert(result2.pairs[std::make_pair('N','C')]  == 1);
        assert(result2.pairs[std::make_pair('C','N')]  == 1);
        assert(result2.pairs[std::make_pair('N','B')]  == 1);
        assert(result2.pairs[std::make_pair('B','C')]  == 1);
        assert(result2.pairs[std::make_pair('C','H')]  == 1);
        assert(result2.pairs[std::make_pair('H','B')]  == 1);
        assert(result2.pairs[std::make_pair('B','B')]  == 1);
        assert(result2.pairs[std::make_pair('B','N')]  == 1);

        assert(counts2_1.size()          == 4);
        assert(counts2_1['N']            == 3);
        assert(counts2_1['C']            == 2);
        assert(counts2_1['B']            == 3);
        assert(counts2_1['H']            == 1);
    }
}


auto getDiff(Polymer const&polymer)
{
    auto const counts=polymer.getCounts();

    auto max{counts.begin()->second};
    auto min{counts.begin()->second};

    for(auto [c,count] : counts)
    {
        max = std::max(max,count);
        min = std::min(min,count);
    }
    return max-min;
}



void test2()
{
    test1Data.clear();
    test1Data.seekg(0);


    auto [polymer, rules] = parseInput(test1Data);
        
    for(int step=0; step<10;step++)
    {
        polymer = polymerise(polymer,rules);
    }

    auto counts=polymer.getCounts();


    assert(counts['B'] == 1749);
    assert(counts['C'] ==  298);
    assert(counts['H'] ==  161);
    assert(counts['N'] ==  865);

    assert(getDiff(polymer) == 1588);
}


void real()
{
    auto [polymer, rules] = parseInput(realData);

    for(int step=0; step<10;step++)
    {
        polymer = polymerise(polymer,rules);
    }

    std::cout << std::format("Part 1 : {}\n",getDiff(polymer));


    for(int step=0; step<30;step++)
    {
        polymer = polymerise(polymer,rules);
    }

    std::cout << std::format("Part 2 : {}\n",getDiff(polymer));
}

int main()
try
{
    test1();
    test2();

    real();

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}