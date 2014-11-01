#include "GRASP.h"

bool stopCriterion(int iteration){
	return iteration >= GRASP_MAX_ITERATIONS;
}

Solution greedyRandomizedConstruction(CitiesData cities){
	return getRandomSolution(cities); // TODO: make it greedy
}

Solution localSearch(Solution solution)
{
	return solution; // TODO: implement
}

Solution GRASP_findPath(CitiesData cities){ // Todo: ...
	Solution bestSolution;
	Solution solution;

	solution = greedyRandomizedConstruction(cities);
	bestSolution = solution;

	int i;
	for(int i = 1; !stopCriterion(i); i++){
		solution = greedyRandomizedConstruction(cities);
		solution = localSearch(solution);
		bestSolution = getBetterSolution(solution,bestSolution);
	}

	return bestSolution;
}