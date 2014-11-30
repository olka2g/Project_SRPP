#include "AlgorithmController.h"
#include <math.h>
#include <vector>
#include "FlowerAlgorithm.h"

Solution findPath(CitiesData cities){
	return Flower_findPath(cities);
	//return GRASP_findPath(cities);
	//return Tabu_findPath(cities);
}

#pragma region functions common for all algorithms

float distanceBetween(City c1, City c2)
{
	float x1 = c1.location.x;
	float y1 = c1.location.y;
	float x2 = c2.location.x;
	float y2 = c2.location.y;
	float xd = x2-x1;
	float yd = y2-y1;
	float result = sqrt(xd*xd + yd*yd);
	return result;
}


float getSingleRouteLength(Route& route)
{
	float route_length = 0;

	// For each city on route
	for (int j = 1; j < route.num_cities; j++)
	{
		route_length +=
			distanceBetween(
			route.cities[j-1],
			route.cities[j]
		);
	}

	return route_length;
}

//Returns true if two routes are exactly the same (contains all cities in the same order).
//Caution! It returns false if route2 is same as route1 but backwards.
bool AreSame(Route& route1, Route& route2)
{
	if (route1.num_cities != route2.num_cities)
		return false;

	for (int i=0; i<route1.num_cities; i++)
	{
		if (route1.cities[i].id != route2.cities[i].id)
			return false;
	}

	return true;
}

float getRoutesLength(Solution solution)
{
	float sum = 0;

	// For each route
	for (int i = 0; i < solution.num_routes; i++)
	{
		float route_length = getSingleRouteLength(solution.routes[i]);

		sum += route_length;
	}

	return sum;
}

void printResults(Solution solution)
{
	printf("%f\n",getRoutesLength(solution));

	printf("%d\n",solution.num_routes);

	for (int i = 0; i < solution.num_routes; i++)
	{
		for (int j = 0; j < solution.routes[i].num_cities; j++)
		{
			printf("%3ld ",solution.routes[i].cities[j].id);
		}

		printf("\n");
	}
}

City randomCity(CitiesData cd)
{
	return cd.cities[(int)(rand() % cd.count)];
}

Solution getBetterSolution(Solution solution, Solution bestSolution)
{
	return getRoutesLength(solution) < getRoutesLength(bestSolution) ? solution : bestSolution;
}

// uniform but slow
long uniformRandomAtMost(long max) {
	unsigned long
		// max <= RAND_MAX < ULONG_MAX, so this is okay.
		num_bins = (unsigned long) max + 1,
		num_rand = (unsigned long) RAND_MAX + 1,
		bin_size = num_rand / num_bins,
		defect   = num_rand % num_bins;

	long x;
	// This is carefully written not to overflow
	while (num_rand - defect <= (unsigned long)(x = rand()));

	// Truncated division is intentional
	return x/bin_size;
}

// uniform but slow
long uniformRandomInRange(long from, long to){
	return from + uniformRandomAtMost(to - from);
}

// fast but non uniform distibution
long fastRandomInRange(long from, long to){
	return from + (rand() % (to-from));
}

Solution getRandomSolution(CitiesData cd){
	Solution s;

	// Liczba tras najmniejsza mo�liwa, je�li ka�da trasa ma k miast.
	// Najwi�ksza mo�liwa je�li ka�da ma po 1 mie�cie.
	s.num_routes = fastRandomInRange((long)ceil((double)cd.count/cd.k),cd.count);

	// Alokuj memori� na ruty
	s.routes = (Route*)malloc(s.num_routes*sizeof(Route));	
	// Wyzeruj alokowan� pami�� i dodaj magazyn jako punkt startowy
	for (int i = 0; i < s.num_routes; i++){
		s.routes[i].num_cities = 1;
		s.routes[i].cities = (City*)malloc(sizeof(City));
		s.routes[i].cities[0] = cd.warehouse; 
	}

	// Ka�demu miastu przypisz kt�r�� z tras	
	std::vector<int> citiesLeft;
	for (int i = 0; i < cd.count; i++)
		citiesLeft.push_back(i);

	while(!citiesLeft.empty())
	{
		for (int i = 0; i < s.num_routes; i++)
		{
			if(citiesLeft.empty())
				break;
			
			// traso wylosuj sobie miasto
			if(s.routes[i].num_cities < cd.k + 1){				
				if(fastRandomInRange(0,100) > 50){
					s.routes[i].num_cities += 1;
					s.routes[i].cities = (City*)realloc(s.routes[i].cities,s.routes[i].num_cities*sizeof(City));
					s.routes[i].cities[s.routes[i].num_cities - 1] = cd.cities[citiesLeft.back()];
					citiesLeft.pop_back();
				}
			}
		}
	}

	// Dodaj magazyn jako punkt ko�cowy
	for (int i = 0; i < s.num_routes; i++){
		s.routes[i].num_cities += 1;
		s.routes[i].cities = (City*)realloc(s.routes[i].cities,s.routes[i].num_cities*sizeof(City));
		s.routes[i].cities[s.routes[i].num_cities - 1] = cd.warehouse;
	}

	return s;
}

//support for sortAndPrintAllCities function
int cityQuickSortComparison (const void * a, const void * b)
{
	return ((*(City*)a).id - (*(City*)b).id);
}

//debug function - useful to check if every city is included and there's no repetition
void sortAndPrintAllCities(const CitiesData& area, const Solution& solution)
{
	City* cities = (City*)malloc(sizeof(City)*area.count);
	int cityIterator = 0;

	//copy all the cities to the separate array
	for (int i=0; i<solution.num_routes; i++)
	{
		for (int j=1; j<solution.routes[i].num_cities-1; j++)
		{
			cities[cityIterator] = solution.routes[i].cities[j];
			cityIterator++;
		}
	}

	//sort this array
	qsort(cities, cityIterator, sizeof(City),cityQuickSortComparison);

	//print ids of sorted cities (10 in row)
	printf("Uzyte miasta:\n");
	for (int i=0; i<cityIterator; i++)
	{
		printf("%02ld ", cities[i].id);
		if ((i+1) % 10 == 0)
			printf("\n");
	}
	printf("\n");

	free(cities);
}

#pragma endregion