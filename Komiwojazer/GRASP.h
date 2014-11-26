#pragma once
#include "DataTypes.h"
#include <stdlib.h>
#include "AlgorithmController.h"

// max iterations 1000 advised
#define GRASP_MAX_ITERATIONS 2 /*1000*/

// inverse greediness
#define GRASP_ALPHA 0.2

typedef struct Candidate{
	City c;
	double incrementalCost;
}Candidate;

Solution GRASP_findPath(CitiesData cities);