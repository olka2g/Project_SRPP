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



SolutionCandidate swapCitiesIn(const Solution& solution, City city1, City city2)
{
	SolutionCandidate newSolution;
	newSolution.solution = solution;
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


void generateNeighbourhood(Solution& baseSolution, std::vector<SolutionCandidate>& neighbourhood, CitiesData& area)
{
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


int cmpfunc (const void * a, const void * b)
{
	return ((*(City*)a).id - (*(City*)b).id);
}

void sortAndPrint(CitiesData area, Solution solution)
{
	City* cities = (City*)malloc(sizeof(City)*area.count);
	int cityIterator = 0;

	for (int i=0; i<solution.num_routes; i++)
	{
		for (int j=1; j<solution.routes[i].num_cities-1; j++)
		{
			cities[cityIterator] = solution.routes[i].cities[j];
			cityIterator++;
		}
	}

	qsort(cities, cityIterator, sizeof(City),cmpfunc);

	for (int i=0; i<cityIterator; i++)
	{
		printf("%d ", cities[i].id);
	}
}



Solution Tabu_findPath(CitiesData cities)
{
	int tabuSize = TABU_DURATION_TIME * 2;
	int i = 0;
	std::vector<TabuItem> tabuList;
	std::vector<SolutionCandidate> neighbourhood;

	Solution baseSolution = getNearestNeighbourSolution(cities);
	/////DEBUG
	printResults(baseSolution);
	sortAndPrint(cities, baseSolution);
	///////////////
	Solution bestSolution = baseSolution;
	float bestCost = getRoutesLength(bestSolution);
	SolutionCandidate baseSolutionCandidate;

	while(!shouldStop(i))
	{

		generateNeighbourhood(baseSolution, neighbourhood, cities);
		baseSolutionCandidate = getBestSolution(neighbourhood, tabuList);
		updateTabu(tabuList, baseSolutionCandidate);
		bestSolution = getBetterSolution(baseSolutionCandidate.solution, bestSolution);
		baseSolution = baseSolutionCandidate.solution;
		i++;
	}

	

	return bestSolution;
}