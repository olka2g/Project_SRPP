#include "GRASP.h"
#include "Visualization.h"

bool stopCriterion(int iteration){
	return iteration >= GRASP_MAX_ITERATIONS;
}

Solution greedyRandomizedConstruction(CitiesData cities){
	return getRandomSolution(cities); // TODO: make it greedy
}

void swapCities(City* c, City* d){
	City tmp;
	tmp.id = c->id;
	tmp.location = c->location;

	c->id = d->id;
	c->location = d->location;

	d->id = tmp.id;
	d->location = tmp.location;		 
}

void copyRoute(Route &dst, Route &src)
{
	dst.cities = (City*)malloc(sizeof(City)*src.num_cities);
	memcpy(dst.cities,src.cities,src.num_cities*sizeof(City));
	dst.num_cities = src.num_cities;
}

void getBestPermutation(Route& r,int i, Route& best) { 
	if (r.num_cities - 1 == i){		
		// if better than best swap
		if(getSingleRouteLength(r) < getSingleRouteLength(best)){
			copyRoute(best,r);
		}
		return;
	}
	int j = i;
	for (j = i; j < r.num_cities - 1; j++) { 
		swapCities(&r.cities[i],&r.cities[j]);
		getBestPermutation(r,i+1,best);
		swapCities(&r.cities[i],&r.cities[j]);
	}
	return;
}


void optimizeRoute(Route& r)
{
	// if route has less than 3 cities (+2 times warehouse) no improvement will occur
	if(r.num_cities > 4){		
		Route cpy;
		copyRoute(cpy, r);
		getBestPermutation(cpy,1,r);
	}
}

Solution getLocallyOptimized(Solution solution)
{
	//float before = getRoutesLength(solution);	
	
	for (int i = 0; i < solution.num_routes; i++)
	{
		// get shortest variation of each route
		optimizeRoute(solution.routes[i]);
	}

	//float gain = before - getRoutesLength(solution);
	
	return solution;
}

Solution GRASP_findPath(CitiesData cities){ // Todo: ...
	Solution bestSolution;
	Solution solution;

	solution = greedyRandomizedConstruction(cities);
	bestSolution = solution;

	int i;
	for(int i = 1; !stopCriterion(i); i++){
		solution = greedyRandomizedConstruction(cities);
		solution = getLocallyOptimized(solution);
		bestSolution = getBetterSolution(solution,bestSolution);
	}	

	return bestSolution;
}