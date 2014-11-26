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


Solution twoOpt(Solution s){
	for (int i = 0; i < s.num_routes; i++)
	{
		s.routes[i] = twoOpt(s.routes[i]);
	}
	return s;
}

Route twoOpt(Route r){
	float gain = 1.0;
	float best = FLT_MAX;

	while(gain > 0) {       
		best = getSingleRouteLength(r);
		
		if(r.num_cities < 4) break;

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