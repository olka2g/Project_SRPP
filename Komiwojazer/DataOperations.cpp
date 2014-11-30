#include "DataOperations.h"
#include <vector>
#include <algorithm>

std::vector<closeNeighborhood> getNeighborhoods(CitiesData cd, int k){
	std::vector<Candidate> cities;
	for (int j = 0; j < cd.count; j++)
	{
		Candidate c;
		c.c = cd.cities[j];
		c.incrementalCost = 0;
		cities.push_back(c);
	}
	
	std::vector<closeNeighborhood> nbs;

	for (int i = 0; i < cd.count; i++)
	{
		closeNeighborhood cn;
		cn.c = cd.cities[i];
		cn.n_neighbors = 0;
		cn.neighbors = NULL;

		nbs.push_back(cn);
	}

	for (int i = 0; i < cd.count; i++)
	{
		// policz odleglosc kazdego miasta od aktualnego
		for (int cand = 0; cand < cities.size(); cand++)
		{
			cities.at(cand).incrementalCost = distanceBetween(cd.cities[i],cities.at(cand).c);
		}
		std::sort(cities.begin(), cities.end(),Candidate_compareByCost);
				
		nbs[i].n_neighbors = k;
		nbs[i].neighbors = (closeNeighborhood**)malloc(sizeof(closeNeighborhood*)*k);
		for (int nb = 0; nb < k; nb++)
		{
			nbs[i].neighbors[nb] = &nbs.at(nb);
		}
	}

	return nbs;
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

City getCity(const CitiesData& area, int x, int y)
{
	if (area.warehouse.location.x == x && area.warehouse.location.y == y)
		return area.warehouse;
	
	for (int i=0; i<area.count; i++)
	{
		if (area.cities[i].location.x == x && area.cities[i].location.y == y)
			return area.cities[i];
	}
	City nullCity;
	nullCity.id = -1;

	return nullCity;
}

City getCity(const CitiesData& area, int id)
{
	for (int i=0; i<area.count; i++)
	{
		if (area.cities[i].id == id)
			return area.cities[i];
	}
	City nullCity;
	nullCity.id = -1;

	return nullCity;
}

void insertRouteIn(Solution& solution, City* cityBuffer, int bufferCount)
{
	solution.num_routes++;
	if (solution.routes == NULL)
	{
		solution.routes = (Route*) malloc(sizeof(Route)*solution.num_routes);
	}
	else
	{
		solution.routes = (Route*) realloc(solution.routes, sizeof(Route)*solution.num_routes);
	}

	solution.routes[solution.num_routes-1].cities = cityBuffer;
	solution.routes[solution.num_routes-1].num_cities = bufferCount;

}

CitiesData copyCitiesData(const CitiesData &initArea)
{
	//initialize new structure
	CitiesData area;
	area.count = initArea.count;
	area.k = initArea.k;
	area.warehouse = initArea.warehouse;
	area.cities = (City*)malloc(sizeof(City)*area.count);

	//rewrite array of cities
	for (int i=0; i<area.count; i++)
	{
		area.cities[i] = initArea.cities[i];
	}

	return area;
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

Route getLongestRouteIn(const Solution& solution, int skipFirstLongest /*= 0*/)
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