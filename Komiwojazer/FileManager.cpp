#include "FileManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

char* chooseFile()
{
	return "data\\100_k=4";
}

CitiesData loadFile(char* filePath)
{
	FILE* fr;
	int n;	
	const int BUFF_SIZE = 255;
	char line[BUFF_SIZE];
	
	CitiesData citiesData;
	citiesData.count = 0;
	
	fr = fopen (filePath, "rt");  
	
	// Read first line - k
	fgets(line, BUFF_SIZE, fr);
	sscanf (line, "%d", &(citiesData.k));

	citiesData.cities = NULL;

	// Read second line - warehouse coordinates
	fgets(line, BUFF_SIZE, fr);
	sscanf (line, "%d %d", &(citiesData.warehouse.x), &(citiesData.warehouse.y));

	while(fgets(line, BUFF_SIZE, fr) != NULL)
	{		
		Point city;
		
		// Scan next city's coordinates
		sscanf (line, "%d %d", &(city.x), &(city.y));
		
		citiesData.cities = (Point*)realloc(citiesData.cities,(++citiesData.count)*sizeof(Point));

		citiesData.cities[citiesData.count-1] = city;
	}
	fclose(fr);

	return citiesData;
}