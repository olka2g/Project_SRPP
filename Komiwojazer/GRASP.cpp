#include "GRASP.h"
#include "Visualization.h"
#include <algorithm>

bool stopCriterion(int iteration){
	return iteration >= GRASP_MAX_ITERATIONS;
}

bool solutionIncomplete(std::vector<City>& citiesToGo){	
	return citiesToGo.size() > 0;
}

template <typename T>
T take(std::vector<T>& v, typename std::vector<T>::size_type n)
{
	std::swap(v[n], v.back());
	T t = v.back();
	v.pop_back();
	return t;
}

bool Candidate_compareByCost(const Candidate &a, const Candidate &b)
{
	return a.incrementalCost < b.incrementalCost;
}

std::vector<Candidate> buildRCL(std::vector<City>& citiesToGo, Solution s, CitiesData cd)
{	
	std::vector<Candidate> RCL;
	std::vector<Candidate> allCandidates;
	
	// evaluate (myopic) incremental cost based on the last element of solution
	for (int i = 0; i < citiesToGo.size(); i++)
	{
		Candidate c;
		c.c = citiesToGo.at(i);
				
		if(s.num_routes == 0 || s.routes[s.num_routes-1].num_cities == cd.k + 2){
			// will be the first city on the route -> incremental cost == route length 
			
			
			// TODO: WA¯NE. sporbowac wsadzic kazde miasto do kazdej trasy gdzie jest miejsce 
			// ( i na roznych pozycjach) ( tez tych pustych) policzyc ile tras optymalnie
			
			
			c.incrementalCost = distanceBetween(cd.warehouse,c.c) * 2.0;
		}else{
			// will not be the first-> in that case calculate incremental cost
			// which is the difference between route length before adding city and after
			Route tmp; 
			copyRoute(tmp,s.routes[s.num_routes-1]);

			double costBefore = getSingleRouteLength(tmp);
			
			tmp.cities = (City*)realloc(tmp.cities,(tmp.num_cities + 1) * sizeof(City));
			tmp.cities[tmp.num_cities - 2] = c.c;
			tmp.cities[tmp.num_cities - 1] = cd.warehouse;
			
			double costAfter = getSingleRouteLength(tmp);
			
			free(tmp.cities);

			c.incrementalCost = costAfter - costBefore;
		}		
		
		allCandidates.push_back(c);
	}

	std::sort(allCandidates.begin(), allCandidates.end(),Candidate_compareByCost);
	// TODO: sort all candidates by cost and choose alpha * 100% of them for the RCL
	for (int j = 0; j < allCandidates.size(); j++)
	{
		if(allCandidates.at(j).incrementalCost < 100 || RCL.size() < 10) // TODO:
			RCL.push_back(allCandidates.at(j));
	}
	

	return RCL;
}

Solution addRandomCandidateToSolution(std::vector<Candidate>& RCL,std::vector<City>& citiesToGo, Solution s, CitiesData cd) // TODO: test
{
	if(RCL.size() < 1) return s;
	
	// choose and remove from RCL and citiesToGo
	int ind = fastRandomInRange(0,RCL.size());
	int i;
	Candidate chosen = take(RCL,ind);
	for (i = 0; i < citiesToGo.size(); i++)
	{
		if(citiesToGo.at(i).id == chosen.c.id){
			ind = i;
			break;
		}
	}
	take(citiesToGo,ind);

	Solution r = s;

	i = r.num_routes - 1;
	if(i >= 0 && r.routes[i].num_cities < cd.k + 2){
		// dodajemy do ostatniej, niepe³nej trasy
		r.routes[i].num_cities++;
		r.routes[i].cities = (City*)realloc((void*)r.routes[i].cities,r.routes[i].num_cities*sizeof(City));
		r.routes[i].cities[r.routes[i].num_cities-2] = chosen.c;
		r.routes[i].cities[r.routes[i].num_cities-1] = cd.warehouse;
		return r;
	}else{
		// ostatnia trasa jest pe³na, wiêc tworzymy now¹ z jednym elementem
		i++;
		r.num_routes++;
		if(i > 0){
			r.routes = (Route*)realloc(r.routes,r.num_routes*sizeof(Route));
		}else{
			r.routes = (Route*)malloc(r.num_routes*sizeof(Route));
		}

		r.routes[i].num_cities = 3;
		r.routes[i].cities = (City*)malloc(3*sizeof(City));
		r.routes[i].cities[0] = cd.warehouse;
		r.routes[i].cities[1] = chosen.c;
		r.routes[i].cities[2] = cd.warehouse;
	}

	return r;
}

Solution greedyRandomizedConstruction(CitiesData cities){
	Solution s;
	s.num_routes = 0;
	std::vector<City> citiesToGo;
	for (int i = 0; i < cities.count; i++)
		citiesToGo.push_back(cities.cities[i]);

	while(solutionIncomplete(citiesToGo)){
		std::vector<Candidate> RCL = buildRCL(citiesToGo,s,cities);
		s = addRandomCandidateToSolution(RCL,citiesToGo,s,cities);
	}
	return s;
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

Solution fineTuneSolution(Solution solution)
{
	for (int i = 0; i < solution.num_routes; i++)
	{
		// get shortest variation of each route
		optimizeRoute(solution.routes[i]);
	}
	return solution;
}

Solution localSearch(Solution solution)
{
	// nie wiem
	return solution; // TODO: this
}

Solution GRASP_findPath(CitiesData cities){
	Solution bestSolution;
	Solution solution;

	solution = greedyRandomizedConstruction(cities);
	bestSolution = solution;

	printf("starting fitness: %f\n",getRoutesLength(solution));

	int i;
	for(int i = 1; !stopCriterion(i); i++){
		// greedy randomized construction
		solution = greedyRandomizedConstruction(cities);

		// neighborhood search
		solution = localSearch(solution);		

		double gain = getRoutesLength(bestSolution) - getRoutesLength(solution);
		if(gain > 0)
			printf("round %d. gained: %f\n",i,gain);

		bestSolution = getBetterSolution(solution,bestSolution);

		if(gain <= 0.0)destroySolution(solution);
	}	
	printf("\n");

	// fine tuning
	if(cities.k < 8)
		solution = fineTuneSolution(solution);

	return bestSolution;
}