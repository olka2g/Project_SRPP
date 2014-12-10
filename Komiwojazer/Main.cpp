#include "FileManager.h"
#include "DataTypes.h"
#include "AlgorithmController.h"
#include "Visualization.h"
#include <string>

using namespace std;
using namespace cimg_library;



int main()
{
	srand(clock());

	while(true){
		system("cls");

		string path = chooseFile();
		CitiesData citiesData = loadFile(path);

		createCitiesDisplay(citiesData);	

		clock_t start = clock();
		Solution solution = findPath(citiesData);
		clock_t end = clock();

		printResults(solution);

		printf("\nTime elapsed: %3.3f s\n",((double)(end-start))/CLOCKS_PER_SEC);

		//displayResultAnimated(solution);
		displayResult(solution);


		printf("\nCare for another round? Y/N\n");
		char r = getchar();
		if(r == 'y' || r == 'Y') 
			continue;
		else 
			break;
	}

	return 0;
}