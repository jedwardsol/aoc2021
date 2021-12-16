#define NOMINMAX
#include <cassert>
#include <cstdint>

#include <string>
#include <charconv>
#include <vector>

#include <iostream>
#include <format>

#include <numeric>

using namespace std::literals;
#include "include/thrower.h"

extern std::string realData;


class BitStream
{
    std::string         binary;
    std::string_view    bits;

    BitStream(BitStream const &)              = delete;
    BitStream(BitStream      &&)              = delete;
    BitStream &operator=(BitStream const  &)  = delete;
    BitStream &operator=(BitStream       &&)  = delete;

public:

    explicit BitStream(std::string const &hex) : binary{toBinary(hex)}, bits{binary}
    {}

    explicit BitStream(std::string_view   bits) : bits{bits}
    {}

    bool empty()
    {
        return bits.empty();
    }

    std::string toBinary(std::string const &hex)
    {
        std::string binary;

        for(auto c : hex)
        {
            assert(isdigit(c)||(isalpha(c)&&isupper(c)));

            if(std::isdigit(c))
            {
                binary += std::format("{:04b}",c-'0');
            }
            else 
            {
                binary += std::format("{:04b}",c-'A'+10);
            }
        }

        return binary;
    }

    uint32_t extractNumber(int bitCount)
    {
        uint32_t number{};

        assert(bits.size() >=bitCount);

        auto result = std::from_chars(bits.data(),bits.data()+bitCount,number,2);
        bits.remove_prefix(bitCount);

        assert(result.ec == std::errc{});

        return number;
    }

    std::string_view extractBits(int bitCount)
    {
        assert(bits.size() >=bitCount);

        auto result = bits.substr(0,bitCount);
        bits.remove_prefix(bitCount);
        return result;
    }
};


class Packet
{
    enum class Type
    {
        sum,
        product,
        minimum,
        maximum,
        literal,
        greaterThan,
        lessThan,
        equals,
    };

    uint32_t                version;
    Type                    type;

    uint64_t                value{};
    std::vector<Packet>     subpackets;

public:

    Packet(BitStream &bitStream) : version{bitStream.extractNumber(3)},
                                   type   {static_cast<Type>(bitStream.extractNumber(3))}
                            
    {
        if(type==Type::literal)
        {
            bool    lastGroup;
            int     groupCount{};    
            do
            {
                lastGroup  = !bitStream.extractNumber(1);
                value    <<=  4;
                value     |=  bitStream.extractNumber(4);

                groupCount++;

            } while(!lastGroup);

            assert(groupCount <= sizeof(value)*8/4);
        }
        else
        {
            auto const lengthTypeId = bitStream.extractNumber(1);

            if(lengthTypeId == 0)
            {
                // bit count case
                auto const bitCount = bitStream.extractNumber(15);

                auto subStream     = BitStream{bitStream.extractBits(bitCount)};


                while(!subStream.empty())
                {
                    subpackets.emplace_back(subStream);
                }
            }
            else
            {
                // sub-packet count case
                auto const subPacketCount = bitStream.extractNumber(11);

                for(auto i=0u;i<subPacketCount;i++)
                {
                    subpackets.emplace_back(bitStream);
                }
            }
        }
    }

    uint64_t    evaluate() const
    {
        uint64_t    result{};

        switch(type)
        {
        case Type::literal:
            result = value;
            break;                


        case Type::sum:
            
            assert(subpackets.size());
            for(auto &child : subpackets)
            {
                result+=child.evaluate();
            }
            break;

        case Type::product:
            assert(subpackets.size());
            result=1;
            for(auto &child : subpackets)
            {
                result*=child.evaluate();
            }
            break;

        case Type::maximum:
            assert(subpackets.size());
            result= std::numeric_limits<uint64_t>::min();
            for(auto &child : subpackets)
            {
                result = std::max(result, child.evaluate());
            }
            break;


        case Type::minimum:
            assert(subpackets.size());
            result= std::numeric_limits<uint64_t>::max();
            for(auto &child : subpackets)
            {
                result = std::min(result, child.evaluate());
            }
            break;


        case Type::equals:
            assert(subpackets.size()==2);
            result = (subpackets[0].evaluate() == subpackets[1].evaluate());
            break;

        case Type::greaterThan:
            assert(subpackets.size()==2);
            result = (subpackets[0].evaluate() > subpackets[1].evaluate());
            break;

        case Type::lessThan:
            assert(subpackets.size()==2);
            result = (subpackets[0].evaluate() < subpackets[1].evaluate());
            break;


        default:
            throw_runtime_error("Evaluating unknown type");
        }

        return result;
    }


    uint32_t sumVersions() const
    {
        auto sum = version;

        for(auto const &child : subpackets)
        {
            sum+=child.sumVersions();
        }

        return sum;
    }
};



int main()
try
{
    BitStream       bitStream{realData};
    Packet  const   packet   {bitStream};
    
    std::cout << "Part 1 : " << packet.sumVersions() << "\n";
    std::cout << "Part 2 : " << packet.evaluate()   << "\n";

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}