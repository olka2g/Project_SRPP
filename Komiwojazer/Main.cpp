#include "FileManager.h"
#include "DataTypes.h"
#include "AlgorithmController.h"
#include "Visualization.h"
//
//#include <stdio.h>
//#include <math.h>
//#include <time.h>

using namespace cimg_library;



int main()
{
	srand(clock());

	char* path = chooseFile();
	CitiesData citiesData = loadFile(path);
	
	createCitiesDisplay(citiesData);	
		
	clock_t start = clock();
	Solution solution = findPath(citiesData);
	clock_t end = clock();

	printResults(solution);

	printf("\nTime elapsed: %3.3f s\n",((double)(end-start))/CLOCKS_PER_SEC);

	displayResultAnimated(solution);
	//displayResult(solution);

	//getchar();

	return 0;
}