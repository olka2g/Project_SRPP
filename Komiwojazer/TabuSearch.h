#pragma once
#include "AlgorithmController.h"
#include "DataTypes.h"
#include "NearestNeighbour.h"
#include "DataOperations.h"

#include <stdlib.h>
#include <vector>

//#define TABU_DURATION_TIME 60
#define MAX_ITERATIONS 200		//iterations in single cycle
#define TABU_CYCLE_INTERVAL 5	//interval between next cities taken into account while constructing initial solution



Solution Tabu_findPath(const CitiesData& cities);