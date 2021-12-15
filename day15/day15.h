#pragma once

#include <vector>
#include <sstream>

using Risk=int;
using Cave= std::vector<std::vector<Risk>>;


int downRightOnly (Cave  const &cave);
int Dijkstra      (Cave  const &cave);



void printSmall   (Cave const &cave);
void printExtended(Cave const &cave);


extern std::istringstream unitData;
extern std::istringstream hardData;
extern std::istringstream testData;
extern std::istringstream realData;
