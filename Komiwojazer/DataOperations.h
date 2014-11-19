#pragma once

#include "DataTypes.h"
#include "AlgorithmController.h"

#include <stdlib.h>

/*
	Returns city of given id (copy - type City is "pointer free") from given area.
	If it cannot find this city in the area, returns -1.
*/
City getCity(const CitiesData& area, int id);

/*
	Changes size of solution, modifies number of routes (must be initialized, even if it's 0)
	and adds cityBuffer to the array inside the solution. To avoid thousands of reallocations, it uses
	directly cityBuffer (does not copy it) thus - do not change it latter! Once used here, its gone. malloc it again.
	bufferCount - number of cities in the buffer.
*/
void insertRouteIn(Solution& solution, City* cityBuffer, int bufferCount);

/*
	Creates the copy of CitiesData given - allocates all important memory and deals with pointers.
*/
CitiesData copyCitiesData(const CitiesData &initArea);

/*
	Creates the copy of Solution given - allocates all important memory and deals with pointers.
*/
Solution copySolution(const Solution& solution);

/*
	Frees all the memory allocated for the solution.
*/
void destroySolution(Solution& solution);

/*
	Finds longest route in the solution and returns it (caution - type Route is NOT "pointer-free", so use
	founded longest route wisely, especially - do not free the array inside).
	Parameter skipFirstLongest for now does nothing. But it is supposed to specify which route we want - for example third longest.
*/
Route getLongestRouteIn(const Solution& solution, int skipFirstLongest = 0);