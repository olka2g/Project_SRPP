#pragma once
#include "DataTypes.h"
#include "GRASP.h"
#include "TabuSearch.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctime>

Solution findPath(CitiesData cities);

float distanceBetween(City c1, City c2);

float getRoutesLength(Solution solution);

float getSingleRouteLength(Route& route);

bool AreSame(Route& route1, Route& route2);

void printResults(Solution solution);

City randomCity(CitiesData cd);

Solution getBetterSolution(Solution solution, Solution bestSolution);

Solution getRandomSolution(CitiesData cd);

void sortAndPrintAllCities(const CitiesData& area, const Solution& solution);

int cityQuickSortComparison (const void * a, const void * b); //might be useful if you want to sort array of cities (like the whole CityData) during debugging