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
	return (iteration < MAX_ITERATIONS);
}

City getNeighbour(CitiesData area, City city)
{
	City nearest = area.cities[0];
	float distance = distanceBetween(city, nearest);

	for (int i = 1; i<area.count; i++)
	{
		if (distanceBetween(city, area.cities[i]) < distance)
		{
			distance = distanceBetween(city, area.cities[i]);
			nearest = area.cities[i];
		}
	}

	return nearest;
}

SolutionCandidate swapCitiesIn(Solution solution, City city1, City city2)
{
	SolutionCandidate newSolution;
	newSolution.solution = solution;
	newSolution.numberOfModifications = 0;
	newSolution.modifiedRoutes = (Route*) malloc(sizeof(Route)*2);

	for (int i = 0; i<solution.num_routes; i++)
	{
		for (int j= 0 ; j<solution.routes[i].num_cities; j++)
		{
			if (solution.routes[i].cities[j].id == city1.id)
			{
				newSolution.solution.routes[i].cities[j] = city2;
				newSolution.modifiedRoutes[newSolution.numberOfModifications] = newSolution.solution.routes[i];
				newSolution.numberOfModifications++;
			}

			else if (solution.routes[i].cities[j].id == city2.id)
			{
				newSolution.solution.routes[i].cities[j] = city1;
				newSolution.modifiedRoutes[newSolution.numberOfModifications] = newSolution.solution.routes[i];
				newSolution.numberOfModifications++;
			}
		}

		if (newSolution.numberOfModifications == 2)
			break;
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


void generateNeighbourhood(Solution baseSolution, std::vector<SolutionCandidate> neighbourhood, CitiesData area)
{
	neighbourhood.clear();
	Route longest = getLongestRouteIn(baseSolution);
	City neighbourCity;
	
	for (int i = 0; i< longest.num_cities; i++)
	{
		neighbourCity = getNeighbour(area, longest.cities[i]);
		neighbourhood.push_back(swapCitiesIn(baseSolution, longest.cities[i], neighbourCity));
	}

}


bool isTabu(SolutionCandidate candidate, TabuItem* tabuList, int tabuSize)
{
	for (int i=0; i<tabuSize; i++)
	{
		for (int j=0; j<candidate.numberOfModifications; j++)
		{
			if  (AreSame(candidate.modifiedRoutes[j], tabuList[i].tabuRoute))
				return true;
		}
	}
	
	return false;
}

SolutionCandidate getBestSolution(std::vector<SolutionCandidate> neighbourhood, TabuItem* tabuList, int tabuSize)
{
	SolutionCandidate bestCandidate = neighbourhood[0];
	float lowestCost = getRoutesLength(bestCandidate.solution);

	for (int i=1; i<neighbourhood.size(); i++)
	{
		if (getRoutesLength(neighbourhood[i].solution) < lowestCost)
		{
			if (!isTabu(neighbourhood[i], tabuList, tabuSize))
			{
				bestCandidate = neighbourhood[i];
				lowestCost = getRoutesLength(bestCandidate.solution);
			}
		}
	}

	return bestCandidate;
}

void updateTabu(TabuItem* tabuList, int tabuSize, SolutionCandidate baseSolutionCandidate)
{
	for (int i=0; i<tabuSize; i++)
	{
		tabuList[i].iterationLeft--;
	}

	for (int i=0; i<tabuSize; i++)
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
}


Solution Tabu_findPath(CitiesData cities)
{
	int tabuSize = TABU_DURATION_TIME * 2;
	int i = 0;
	TabuItem* tabuList = (TabuItem*) malloc(sizeof(TabuItem)*tabuSize);
	std::vector<SolutionCandidate> neighbourhood;

	Solution baseSolution = getRandomSolution(cities);
	Solution bestSolution = baseSolution;
	float bestCost = getRoutesLength(bestSolution);
	SolutionCandidate baseSolutionCandidate;

	while(!shouldStop(i))
	{

		generateNeighbourhood(baseSolution, neighbourhood, cities);
		baseSolutionCandidate = getBestSolution(neighbourhood, tabuList, tabuSize);
		updateTabu(tabuList, tabuSize, baseSolutionCandidate);
		bestSolution = getBetterSolution(baseSolutionCandidate.solution, bestSolution);
		i++;
	}

	free(tabuList);

	return bestSolution;
}