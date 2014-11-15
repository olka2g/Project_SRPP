#include "NearestNeighbour.h"
#include "DataOperations.h"


//Sets city id to 0 in order to hide it. Use carefully and ALWAYS on copy (no unchanged pointers!)
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

City getNeighbour(const CitiesData& area, City city)
{
	City nearest;
	float distance = FLT_MAX;

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

Solution getNearestNeighbourSolution(const CitiesData& cities, int initialID /*= 1*/)
{
	CitiesData area = copyCitiesData(cities);
	City nextCity = getCity(area, initialID);
	int  citiesUsed, cityInRoute, bufferCount;
	Solution initialSolution;
	initialSolution.num_routes = 0;
	initialSolution.routes = NULL;


	for (citiesUsed=0; citiesUsed<area.count; )
	{
		City* cityBuffer = (City*)malloc(sizeof(City)*(area.k+2));
		cityBuffer[0] = area.warehouse;
		bufferCount = 1;

		for(cityInRoute=1; cityInRoute<=area.k; cityInRoute++)
		{
			//add city
			cityBuffer[cityInRoute] = nextCity;
			bufferCount++;

			//check if its end
			citiesUsed++;
			if (citiesUsed >= area.count)
				break;

			//if not, search for next neighbour to add
			hideCity(area, nextCity.id);
			nextCity = getNeighbour(area, nextCity);
		}

		//add route with warehouse
		cityBuffer[bufferCount++] = area.warehouse;
		insertRouteIn(initialSolution, cityBuffer, bufferCount);
	}

	return initialSolution;
}

