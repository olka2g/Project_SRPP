#include "TabuSearch.h"
#include "TwoOpt.h"
#include "DataOperations.h"

/* Local data structures for the algorithm */
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

/* Stop condition */
bool shouldStop(int iteration)
{
	return (iteration >= MAX_ITERATIONS);
}

/* Swap two cities. It creates SolutionCandidate, where solution with swapped cities is saved with modifications
   that were made (that's why I put it here, locally) */
SolutionCandidate swapCitiesIn(const Solution& solution, City city1, City city2)
{
	SolutionCandidate newSolution;
	newSolution.solution = copySolution(solution);
	int routeID1, routeID2;

	//find each city in original solution and swap it in new one; remember id of changed route
	for (int i = 0; i<solution.num_routes; i++)
	{
		for (int j= 0 ; j<solution.routes[i].num_cities; j++)
		{
			if (solution.routes[i].cities[j].id == city1.id)
			{
				newSolution.solution.routes[i].cities[j] = city2;
				routeID1 = i;
			}
			else if (solution.routes[i].cities[j].id == city2.id)
			{
				newSolution.solution.routes[i].cities[j] = city1;
				routeID2 = i;
			}
		}
		//TODO maybe - check if breaking the loop when found will work faster or not
	}
	
	//check how many routes were changed (1 or 2) and save it in new solution candidate
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

/* Generate solutionCandidates that will be solution's neighbourhood */
void generateNeighbourhood(Solution& baseSolution, std::vector<SolutionCandidate>& neighbourhood, const CitiesData& area)
{
 	for (int i=0; i<neighbourhood.size(); i++)
 	{
		destroySolution(neighbourhood[i].solution);
		//free(neighbourhood[i].modifiedRoutes);
	}
	neighbourhood.clear();

	//find longest route in the solution and swap each of its cities with its neighbour
	Route longest = getLongestRouteIn(baseSolution);
	City neighbourCity;
	
	for (int i = 1; i< longest.num_cities-1; i++)
	{
		neighbourCity = getNeighbour(area, longest.cities[i]);
		neighbourhood.push_back(swapCitiesIn(baseSolution, longest.cities[i], neighbourCity));
	}
}

/* Defines if the candidate is taboo or not (if any of its changed routes is forbidden). 
   TODO: aspiration function here. */
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

/* Finds best solution candidate from neighbourhood. */
SolutionCandidate getBestSolution(std::vector<SolutionCandidate>& neighbourhood, std::vector<TabuItem>& tabuList)
{
	SolutionCandidate bestCandidate = neighbourhood[0];
	float lowestCost = getRoutesLength(bestCandidate.solution);
	int bestCandidateIndex = 0;

	for (int i=1; i<neighbourhood.size(); i++)
	{
		if (getRoutesLength(neighbourhood[i].solution) < lowestCost)
		{
			if (!isTabu(neighbourhood[i], tabuList))
			{
				bestCandidateIndex = i;
				bestCandidate = neighbourhood[i];
				lowestCost = getRoutesLength(bestCandidate.solution);
			}
		}
	}

	take(neighbourhood,bestCandidateIndex);

	return bestCandidate;
}

/* Updates taboo's time of duration and inserts newly chosen candidate's routes.*/
void updateTabu(std::vector<TabuItem>& tabuList, SolutionCandidate& baseSolutionCandidate)
{
	//update iterations left
	for (int i=0; i<tabuList.size(); i++)
	{
		tabuList[i].iterationLeft--;
	}

	//use empty spaces in tabu list
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

	//if any of modifications are left, add it at the end
	while (baseSolutionCandidate.numberOfModifications > 0)
	{
		TabuItem newItem;
		newItem.tabuRoute = baseSolutionCandidate.modifiedRoutes[--baseSolutionCandidate.numberOfModifications];
		newItem.iterationLeft = TABU_DURATION_TIME;
		tabuList.push_back(newItem);
	}
}


Solution Tabu_findPath(const CitiesData& cities)
{
	int numberOfCycles = cities.count / TABU_CYCLE_INTERVAL;
	Solution bestSolution;

	for (int cycle = 0; cycle<numberOfCycles; cycle++)
	{
		int i = 0;
		std::vector<TabuItem> tabuList;
		std::vector<SolutionCandidate> neighbourhood;
		SolutionCandidate baseSolutionCandidate;

		Solution baseSolution = getNearestNeighbourSolution(cities, cycle*TABU_CYCLE_INTERVAL + 1);
		
		/////DEBUG
		printf("%d before %f \n", cycle, getRoutesLength(baseSolution));

		if (!cycle)
			bestSolution = baseSolution;
		else
			bestSolution = getBetterSolution(bestSolution, baseSolution);
		float bestCost = getRoutesLength(bestSolution);

		while(!shouldStop(i))
		{
			generateNeighbourhood(baseSolution, neighbourhood, cities);
			baseSolutionCandidate = getBestSolution(neighbourhood, tabuList);
			updateTabu(tabuList, baseSolutionCandidate);
			bestSolution = getBetterSolution(baseSolutionCandidate.solution, bestSolution);
			Solution tmp = baseSolution;
			baseSolution = baseSolutionCandidate.solution;

			if (tmp.routes != bestSolution.routes)
			{
				destroySolution(tmp);
			}
			i++;
		}

		//destroySolution()

		////DEBUG
		printf("%d after %f \n", cycle, getRoutesLength(bestSolution));

	}
	bestSolution = twoOpt(bestSolution);
	
	////DEBUG
	sortAndPrintAllCities(cities, bestSolution);
	return bestSolution;
}