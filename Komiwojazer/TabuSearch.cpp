#include "TabuSearch.h"


typedef struct TabuItem
{
	Route tabuRoute;
	int timeLeft;
} TabuItem;



Solution Tabu_findPath(CitiesData cities)
{
	int tabuSize = TABU_DURATION_TIME;
	TabuItem* tabuList = (TabuItem*) malloc(sizeof(TabuItem)*tabuSize);


	free(tabuList);
}