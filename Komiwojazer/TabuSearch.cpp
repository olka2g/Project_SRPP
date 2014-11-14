#include "TabuSearch.h"


typedef struct TabuItem
{
	Route tabuRoute;
	int iterationLeft;
} TabuItem;


typedef struct SolutionCandidate
{
	Route* modifiedRoutes;
	int numberOfModifications;
	Solution solution;
} SolutionCandidate;

bool shouldStop(int iteration)
{
	return (iteration >= MAX_ITERATIONS);
}

Solution copySolution(const Solution& solution)
{
	Solution newSolution;
	newSolution.num_routes = solution.num_routes;
	newSolution.routes = (Route*) malloc(sizeof(Route)*solution.num_routes);

	for (int i=0; i<solution.num_routes; i++)
	{
		newSolution.routes[i].cities = (City*) malloc(sizeof(City)*solution.routes[i].num_cities);
		newSolution.routes[i].num_cities = solution.routes[i].num_cities;

		for (int j=0; j<solution.routes[i].num_cities; j++)
		{
			newSolution.routes[i].cities[j] = solution.routes[i].cities[j];
		}
	}

	return newSolution;
}

void destroySolution(Solution& solution)
{
	for (int i=0; i<solution.num_routes; i++)
		free(solution.routes[i].cities);

	free(solution.routes);
}

SolutionCandidate swapCitiesIn(const Solution solution, City city1, City city2)
{
	SolutionCandidate newSolution;
	newSolution.solution = copySolution(solution);
	int routeID1, routeID2;

	for (int i = 0; i<solution.num_routes; i++)
	{
		for (int j= 0 ; j<solution.routes[i].num_cities; j++)
		{
			if (solution.routes[i].cities[j].id == city1.id)
			{
				newSolution.solution.routes[i].cities[j] = city2;
				routeID1 = i;
			}

			if (solution.routes[i].cities[j].id == city2.id)
			{
				newSolution.solution.routes[i].cities[j] = city1;
				routeID2 = i;
			}
		}

		//TODO maybe - check if breaking the loop when found will work faster or not
	}
	if (routeID1 == routeID2)
	{
		newSolution.numberOfModifications = 1;
		newSolution.modifiedRoutes = (Route*) malloc(sizeof(Route));
		newSolution.modifiedRoutes[0] = newSolution.solution.routes[routeID1];
	}
	else
	{
		newSolution.numberOfModifications = 2;
		newSolution.modifiedRoutes = (Route*) malloc(sizeof(Route)*2);
		newSolution.modifiedRoutes[0] = newSolution.solution.routes[routeID1];
		newSolution.modifiedRoutes[1] = newSolution.solution.routes[routeID2];
	}

	return newSolution;
}

Route getLongestRouteIn(Solution solution, int skipFirstLongest = 0)
{
	float longestRouteLength = 0;
	Route longestRoute;
	for (int i = 0; i<solution.num_routes; i++)
	{
		if(longestRouteLength < getSingleRouteLength(solution.routes[i]))
		{
			longestRouteLength = getSingleRouteLength(solution.routes[i]);
			longestRoute = solution.routes[i];
		}
	}

	return longestRoute;
}


void generateNeighbourhood(Solution& baseSolution, std::vector<SolutionCandidate>& neighbourhood, CitiesData area)
{
// 	for (int i=0; i<neighbourhood.size(); i++)
// 		destroySolution(neighbourhood[i].solution);
	neighbourhood.clear();

	Route longest = getLongestRouteIn(baseSolution);
	City neighbourCity;
	
	for (int i = 1; i< longest.num_cities-1; i++)
	{
		neighbourCity = getNeighbour(area, longest.cities[i]);
		neighbourhood.push_back(swapCitiesIn(baseSolution, longest.cities[i], neighbourCity));
	}

}


bool isTabu(SolutionCandidate& candidate, std::vector<TabuItem>& tabuList)
{
	for (int i=0; i<tabuList.size(); i++)
	{
		for (int j=0; j<candidate.numberOfModifications; j++)
		{
			if  (AreSame(candidate.modifiedRoutes[j], tabuList[i].tabuRoute))
				return true;
		}
	}
	
	return false;
}

SolutionCandidate getBestSolution(std::vector<SolutionCandidate>& neighbourhood, std::vector<TabuItem>& tabuList)
{
	SolutionCandidate bestCandidate = neighbourhood[0];
	float lowestCost = getRoutesLength(bestCandidate.solution);

	for (int i=1; i<neighbourhood.size(); i++)
	{
		if (getRoutesLength(neighbourhood[i].solution) < lowestCost)
		{
			if (!isTabu(neighbourhood[i], tabuList))
			{
				bestCandidate = neighbourhood[i];
				lowestCost = getRoutesLength(bestCandidate.solution);
			}
		}
	}

	return bestCandidate;
}

void updateTabu(std::vector<TabuItem>& tabuList, SolutionCandidate& baseSolutionCandidate)
{
	for (int i=0; i<tabuList.size(); i++)
	{
		tabuList[i].iterationLeft--;
	}

	for (int i=0; i<tabuList.size(); i++)
	{
		if (tabuList[i].iterationLeft <= 0)
		{
			if (baseSolutionCandidate.numberOfModifications > 0)
			{
				tabuList[i].tabuRoute = baseSolutionCandidate.modifiedRoutes[--baseSolutionCandidate.numberOfModifications];
				tabuList[i].iterationLeft = TABU_DURATION_TIME;
			}
		}
	}

	while (baseSolutionCandidate.numberOfModifications > 0)
	{
		TabuItem newItem;
		newItem.tabuRoute = baseSolutionCandidate.modifiedRoutes[--baseSolutionCandidate.numberOfModifications];
		newItem.iterationLeft = TABU_DURATION_TIME;
		tabuList.push_back(newItem);
	}
}


Solution Tabu_findPath(CitiesData area)
{
	int numberOfCycles = area.count / TABU_CYCLE_INTERVAL;
	Solution bestSolution;

	for (int cycle = 0; cycle<numberOfCycles; cycle++)
	{
		int i = 0;
		std::vector<TabuItem> tabuList;
		std::vector<SolutionCandidate> neighbourhood;

		Solution baseSolution = getNearestNeighbourSolution(area, cycle*TABU_CYCLE_INTERVAL + 1);
		printf("%d before %f \n", cycle, getRoutesLength(baseSolution));
			// 	qsort(area.cities, area.count, sizeof(City),cityQuickSortComparison);
			// 	for (int g=0; g<area.count; g++)
			// 	{
			// 		printf("%02ld ", area.cities[g].id);
			// 		if ((g+1) % 10 == 0)
			// 			printf("\n");
			// 	}
			// 	printf("\n");
		/////DEBUG
		//printResults(baseSolution);
		//sortAndPrintAllCities(cities, baseSolution);
		///////////////
		if (!cycle)
			bestSolution = baseSolution;
		else
			bestSolution = getBetterSolution(bestSolution, baseSolution);
		float bestCost = getRoutesLength(bestSolution);
		SolutionCandidate baseSolutionCandidate;

		while(!shouldStop(i))
		{
			generateNeighbourhood(baseSolution, neighbourhood, area);
			baseSolutionCandidate = getBestSolution(neighbourhood, tabuList);
			updateTabu(tabuList, baseSolutionCandidate);
			bestSolution = getBetterSolution(baseSolutionCandidate.solution, bestSolution);
			baseSolution = baseSolutionCandidate.solution;
			i++;
		}

		
		printf("%d after %f \n", cycle, getRoutesLength(bestSolution));

	}
	sortAndPrintAllCities(area, bestSolution);
	return bestSolution;
}