#pragma once
#include "DataTypes.h"
#include <stdlib.h>
#include "AlgorithmController.h"

// max iterations 1000 advised
#define GRASP_MAX_ITERATIONS 500 /*1000*/

// inverse greediness
#define GRASP_ALPHA 0.005

typedef struct Candidate{
	City c;
	double incrementalCost;
}Candidate;

bool Candidate_compareByCost(const Candidate &a, const Candidate &b);

Solution GRASP_findPath(CitiesData cities);