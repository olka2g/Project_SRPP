#include "FileManager.h"

string chooseFile()
{	
	string folder = ".\\data\\";
	vector<string> names;
    char search_path[200];
    sprintf(search_path, "%s*.*", folder.c_str());
    WIN32_FIND_DATA fd; 
    HANDLE hFind = ::FindFirstFile(search_path, &fd); 
    if(hFind != INVALID_HANDLE_VALUE) 
    { 
        do 
        { 
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) 
            {
                names.push_back(fd.cFileName);
            }
        }while(::FindNextFile(hFind, &fd)); 
        ::FindClose(hFind); 
    } 

	for(int i = 0; i < names.size();i++){
		printf("%d. \t%s\n",i,names.at(i).c_str());
	}
	
	string userInput;
	int chosen = -1;
	do{
		printf("Choose file:\t");
		getline(cin,userInput);
		try{
			chosen = stoi(userInput.c_str());
		}catch(exception e){
			chosen = -1;
		}
	}while(chosen >= names.size() || chosen < 0);

	system("cls");

	return folder + names.at(chosen);
}

CitiesData loadFile(string filePath)
{
	FILE* fr;
	int n;
	const int BUFF_SIZE = 255;
	char line[BUFF_SIZE];

	CitiesData citiesData;
	citiesData.count = 0;

	fr = fopen (filePath.c_str(), "rt");

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