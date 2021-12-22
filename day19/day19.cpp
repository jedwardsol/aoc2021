#include <cassert>
#include <cstdint>

#include <set>
#include <vector>

#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"

#include "rotations.h"

void testDie();

extern std::istringstream realData;
extern std::istringstream testData;

using Beacon = Location;

struct Scanner
{
    std::set<Beacon>     beacons;
    std::set<Location>   mergedScanners;
};

auto readData(std::istringstream &data)
{
    std::vector<Scanner>    scanners;
    std::string             line;

    while(std::getline(data,line))
    {
        if(line.empty())
        {
            continue;
        }

        if(line.starts_with("--- scanner "s))
        {
            scanners.emplace_back();
        }
        else
        {
            std::istringstream  coords{line};
            Beacon              beacon{};
            char                comma;

            coords >> beacon[0] >> comma >> beacon[1] >> comma >> beacon[2];

            assert(coords);

            scanners.back().beacons.insert(beacon);
        }
    }

    return scanners;
}


auto rotateScanner(Scanner const &scanner, Rotation const &rotation)
{
    Scanner rotatedScanner;

    for(auto const &beacon : scanner.beacons)
    {
        rotatedScanner.beacons.insert ( rotation * beacon);
    }

    return rotatedScanner;
}

auto translateScanner(Scanner const &scanner, Translation const &translation)
{
    Scanner translatedScanner;

    for(auto const &beacon : scanner.beacons)
    {
        translatedScanner.beacons.insert ( beacon + translation);
    }

    return translatedScanner;
}


int countOverlap(Scanner const &masterScanner, Scanner const &scanner)
{
    int count{};

    for(auto const &beacon : scanner.beacons)
    {
        if(masterScanner.beacons.contains(beacon))
        {
            count++;
        }
    }

    return count;
}

bool merge(Scanner &masterScanner, Scanner const &scanner)
{

    for(auto const &rotation : allRotations)
    {
        auto const rotatedScanner = rotateScanner(scanner,rotation);

        for(auto const candidateMasterBeacon : masterScanner.beacons)
        {
            for(auto const candidateBeacon : rotatedScanner.beacons)
            {
                auto const translation       = candidateMasterBeacon - candidateBeacon;
                auto const translatedScanner = translateScanner(rotatedScanner,translation);

                assert(translatedScanner.beacons.contains(candidateMasterBeacon));

                auto overlap = countOverlap(masterScanner, translatedScanner);

                assert(overlap > 0);

                if(overlap >= 12)
                {
                    for(auto const &beacon : translatedScanner.beacons)
                    {
                        masterScanner.beacons.insert(beacon);
                    }

                    masterScanner.mergedScanners.insert(translation);
                    for(auto const &child : translatedScanner.mergedScanners)
                    {
                        masterScanner.mergedScanners.insert(child+translation);
                    }

                    return true;
                }
            }
        }
    }


    return false;;
}


void findMerge(std::vector<Scanner> &scanners)
{
    for(int i=0;i<scanners.size(); i++)
    {
        for(int j=i+1;j < scanners.size(); j++)
        {
            if(merge(scanners[i], scanners[j]))
            {
                scanners.erase(scanners.begin()+j);
                
                return;
            }
        }
    }

    throw_runtime_error("No merge found");
}



void mergeAll(std::vector<Scanner> &scanners)
{
    while(scanners.size() > 1)
    {
        findMerge(scanners);

        std::cout << "Size : " << scanners.size() << "\n";
    }
}



int maxDistance(Scanner const &scanner)
{
    // double count. oh well./

    int maxDistance{};

    for(auto const &dest : scanner.mergedScanners)
    {
        auto translation = Location{0,0,0}-dest;

        auto distance =   std::abs(translation[0])
                        + std::abs(translation[1])
                        + std::abs(translation[2]);

        maxDistance=std::max(maxDistance,distance);
    }



    for(auto const &src : scanner.mergedScanners)
    {
        for(auto const &dest : scanner.mergedScanners)
        {
            auto translation = src-dest;

            auto distance =   std::abs(translation[0])
                            + std::abs(translation[1])
                            + std::abs(translation[2]);

            maxDistance=std::max(maxDistance,distance);
        }
    }


    return maxDistance;
}


int main()
try
{
//  testDie();
//  auto scanners=readData(testData);
    auto scanners=readData(realData);
    mergeAll(scanners);

    std::cout << "Part 1 : " << scanners[0].beacons.size() << "\n";
    std::cout << "Part 2 : " << maxDistance(scanners[0]) << "\n";

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}