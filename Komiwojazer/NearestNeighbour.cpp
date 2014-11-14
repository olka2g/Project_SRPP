#include "NearestNeighbour.h"


//Sets city id to 0 in order to hide it. Use carefully.
void hideCity(CitiesData& area, int id)
{
	for (int i = 0; i<area.count; i++)
	{
		if (area.cities[i].id == id)
		{
			area.cities[i].id = 0;
			return;
		}
	}
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

City getNeighbour(const CitiesData& area, City city)
{
	City nearest;
	float distance = FLT_MAX;//distanceBetween(city, nearest);

	for (int i = 0; i<area.count; i++)
	{
		if (area.cities[i].id > 0)
			if(area.cities[i].id != city.id)
				if(distanceBetween(city, area.cities[i]) < distance)
		{
			distance = distanceBetween(city, area.cities[i]);
			nearest = area.cities[i];
		}
	}

	return nearest;
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

Solution getNearestNeighbourSolution(CitiesData& initArea, int initialID)
{
	Solution initialSolution;
	//copy cities data
	CitiesData area;
	area.count = initArea.count;
	area.k = initArea.k;
	area.warehouse = initArea.warehouse;
	area.cities = (City*)malloc(sizeof(City)*area.count);
	for (int i=0; i<area.count; i++)
	{
		area.cities[i] = initArea.cities[i];
	}


	City nextCity = getCity(area, initialID);
	int  i, j, bufferCount;
	initialSolution.num_routes = 0;
	initialSolution.routes = NULL;


	//for all cities
	for (i=0; i<area.count; )
	{
		City* cityBuffer = (City*)malloc(sizeof(City)*(area.k+2));
		cityBuffer[0] = area.warehouse;
		bufferCount = 1;

		for(j=1; j<=area.k; j++)
		{

// 			if(i>=96)
// 			{
// 				 	qsort(area.cities, area.count, sizeof(City),cityQuickSortComparison);
// 				 	for (int g=0; g<area.count; g++)
// 				 	{
// 				 		printf("%02ld ", area.cities[g].id);
// 				 		if ((g+1) % 10 == 0)
// 				 			printf("\n");
// 				 	}
// 				 	printf("\n");
// 			}

			cityBuffer[j] = nextCity;
			bufferCount++;

			i++;
			if (i >= area.count)
				break;

			hideCity(area, nextCity.id);
			nextCity = getNeighbour(area, nextCity);
		}
		cityBuffer[bufferCount++] = area.warehouse;
		insertRouteIn(initialSolution, cityBuffer, bufferCount);
	}

	return initialSolution;
}

