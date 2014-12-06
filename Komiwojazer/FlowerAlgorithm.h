#pragma once
#include "DataTypes.h"

typedef struct m_Candidate{
	City c;
	double angle;
	double distance;
	double sin_th;
	double cos_th;
}m_Candidate;

Solution Flower_findPath(CitiesData cd);