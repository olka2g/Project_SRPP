#pragma once

#include "DataTypes.h"
#include "AlgorithmController.h"

#include <stdlib.h>
#include <float.h>

City getNeighbour(const CitiesData& area, City city);
Solution getNearestNeighbourSolution(CitiesData& area, int initialID = 1);