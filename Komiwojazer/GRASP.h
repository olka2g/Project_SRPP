#pragma once
#include "DataTypes.h"
#include <stdlib.h>
#include "AlgorithmController.h"

// max iterations 1000 advised
#define GRASP_MAX_ITERATIONS 1000 /*1000*/

// inverse greediness
#define GRASP_ALPHA 0.005

typedef struct Candidate{
	City c;
	double incrementalCost;
}Candidate;

Solution GRASP_findPath(CitiesData cities);