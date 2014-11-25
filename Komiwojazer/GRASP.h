#pragma once
#include "DataTypes.h"
#include <stdlib.h>
#include "AlgorithmController.h"


#define GRASP_MAX_ITERATIONS 50

// greediness
#define GRASP_ALPHA 0.3

typedef struct Candidate{
	City c;
	double incrementalCost;
}Candidate;

Solution GRASP_findPath(CitiesData cities);