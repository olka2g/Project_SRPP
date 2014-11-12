#include "TabuSearch.h"


typedef struct TabuItem
{
	Route tabuRoute;
	int timeLeft;
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

void generateNeighbourhood(Solution baseSolution, std::vector<SolutionCandidate> neighbourhood)
{
	neighbourhood.clear();

}


Solution Tabu_findPath(CitiesData cities)
{
	int tabuSize = TABU_DURATION_TIME;
	int i = 0;
	TabuItem* tabuList = (TabuItem*) malloc(sizeof(TabuItem)*tabuSize);
	std::vector<SolutionCandidate> neighbourhood;

	Solution baseSolution = getRandomSolution(cities);
	Solution bestSolution = baseSolution;
	float bestCost = getRoutesLength(bestSolution);

	while(!shouldStop(i))
	{

		generateNeighbourhood(baseSolution, neighbourhood);
		//baseSolution = getBestSolution(neighbourhood, tabuList, tabuSize);

		i++;
	}

	free(tabuList);

	return bestSolution;
}