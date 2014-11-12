#pragma once
#include "AlgorithmController.h"
#include "DataTypes.h"
#include <stdlib.h>
#include <vector>

#define TABU_DURATION_TIME 5
#define MAX_ITERATIONS 1000

Solution Tabu_findPath(CitiesData cities);