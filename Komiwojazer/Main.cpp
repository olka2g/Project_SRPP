#include "FileManager.h"
#include "DataTypes.h"
#include "AlgorithmController.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

int main()
{
	srand(clock());

	char* path = chooseFile();
	CitiesData citiesData = loadFile(path);
	Solution solution = findPath(citiesData);
	printResults(solution);

	getchar();
	return 0;
}