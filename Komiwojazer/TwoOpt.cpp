#include "TwoOpt.h"
#include "AlgorithmController.h"

using namespace std;


Route swap(Route r, int from, int to)
{		
	Route nr;
	nr.num_cities = r.num_cities;
	nr.cities = (City*)malloc(nr.num_cities*sizeof(City));	

	// 1. take route[0] to route[i-1] and add them in order to new_route
	// 2. take route[i] to route[k] and add them in reverse order to new_route
	// 3. take route[k+1] to end and add them in order to new_route

	memcpy(nr.cities,r.cities,sizeof(City)*r.num_cities);

	for(int i = from, j = to; i <= to; ++i, j--) {		
		nr.cities[i] = r.cities[j];
	}

	return nr;
}




void twoOpt(Route& r,Route& left,Route& right){

	double best = getSingleRouteLength(r) + getSingleRouteLength(left) + getSingleRouteLength(right);
	double gain = 0.0;

	do{
		gain = 0.0;
		for (int i = 1; i < r.num_cities - 1; i++)
		{
			for (int li = 1; li < left.num_cities -1; li++)
			{
				double bef = getSingleRouteLength(r) + getSingleRouteLength(left);
				swapCities(&r.cities[i],&left.cities[li]);
				
				double after = getSingleRouteLength(r) + getSingleRouteLength(left);
				if(bef < after){
					swapCities(&r.cities[i],&left.cities[li]);
				
				}else{
					gain += -after + bef;
					goto br;
				}
			}

			for (int ri = 1; ri < right.num_cities -1; ri++)
			{
				double bef = getSingleRouteLength(r) + getSingleRouteLength(right);
				swapCities(&r.cities[i],&right.cities[ri]);

				double after = getSingleRouteLength(r) + getSingleRouteLength(right);
				if(bef < after){
					swapCities(&r.cities[i],&right.cities[ri]);

				}else{
					gain += -after + bef;
					goto br;
				}
			}
br:			;
		}
	}while(gain > 0);

	//double af = getSingleRouteLength(r) + getSingleRouteLength(left) + getSingleRouteLength(right);
}

Solution twoOpt(Solution& s){
	for (int i = 0; i < s.num_routes; i++)
	{
		s.routes[i] = twoOpt(s.routes[i]);
	}
	return s;
}

Route twoOpt(Route& r){
	float gain = 1.0;
	float best = FLT_MAX;

	while(gain > 0) {       
		best = getSingleRouteLength(r);

		if(r.num_cities < 5) break;

		for (int i = 1; i < r.num_cities - 2; i++) {
			for (int k = i + 1; k < r.num_cities -1; k++) {
				Route new_route = swap(r, i, k);
				float new_distance = getSingleRouteLength(new_route);
				gain = best-new_distance;
				if (gain > 0) {
					best = new_distance;
					free(r.cities);
					r.cities = new_route.cities;
					goto cnt;
				}else{
					free(new_route.cities);
				}
			}
		}
cnt:;	// continue loop
	}
	return r;
}