#pragma once

typedef struct Point {
	int x;
	int y;
} Point;

typedef struct City {
	Point location;
	int id;
} City;

typedef struct CitiesData {
	int k;
	int count;
	City warehouse;
	City* cities;
} CitiesData;

typedef struct Route {
	int num_cities;
	City* cities;
}Route;

typedef struct Solution {
	int num_routes;
	Route* routes;
} Solution;
