#include "DataOperations.h"

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