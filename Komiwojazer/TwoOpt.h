#pragma once
#include <vector>
#include <iostream>
#include <ctime>
#include <algorithm>
#include "DataTypes.h"
#include "DataOperations.h"

Solution twoOpt(Solution& s);

void twoOpt(Route& r,Route& left,Route& right);

Route twoOpt(Route& r);

Route swap(Route r, int from, int to);