#include "FileManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

char* chooseFile()
{
	return "data\\10_k=4";
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
	sscanf (line, "%d %d", &(citiesData.warehouse.location.x), &(citiesData.warehouse.location.y));
	citiesData.warehouse.id = 0;

	while(fgets(line, BUFF_SIZE, fr) != NULL)
	{
		Point point;

		// Scan next city's coordinates
		sscanf (line, "%d %d", &(point.x), &(point.y));

		City city;

		city.id = ++citiesData.count;
		city.location = point;

		citiesData.cities = (City*)realloc(citiesData.cities,(citiesData.count)*sizeof(City));

		citiesData.cities[citiesData.count-1] = city;
	}
	fclose(fr);

	return citiesData;
}