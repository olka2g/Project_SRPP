#pragma once

#include "DataTypes.h"
#include "AlgorithmController.h"

#include <stdlib.h>
#include <float.h>

/*
	This function gets the neighbour - closest city to the one given.
*/
City getNeighbour(const CitiesData& area, City city); 

/*
	This is simple algorithm that can be used to get fine initial solution. It starts with city with given ID and
	creates full routes (max k) by adding always the closest city possible.
*/
Solution getNearestNeighbourSolution(const CitiesData& area, int initialID = 1);