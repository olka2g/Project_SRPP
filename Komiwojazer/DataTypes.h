#pragma once

typedef struct Point {
	int x;
	int y;
} Point;

typedef struct CitiesData {
	int k;
	int count;
	Point warehouse;
	Point* cities;
} CitiesData;