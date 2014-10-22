#pragma once

typedef struct Point {
	int x;
	int y;
};

typedef struct CitiesData {
	int k;
	int count;
	Point warehouse;
	Point cities[];
};