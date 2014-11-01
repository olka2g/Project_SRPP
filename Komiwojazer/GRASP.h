#pragma once
#include "DataTypes.h"
#include <stdlib.h>
#include "AlgorithmController.h"


#define GRASP_MAX_ITERATIONS 1000

// greediness
#define GRASP_ALPHA 0.3

Solution GRASP_findPath(CitiesData cities);