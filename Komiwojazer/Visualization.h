#pragma once

#include "DataTypes.h"
#include "AlgorithmController.h"
#include "lib/cimg/CImg.h"

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <thread>

using namespace cimg_library;

void getBoundaries(CitiesData &cd, int &maxX, int &minX, int &maxY, int &minY);

void createCitiesDisplay(CitiesData& cd);

void displayResult(Solution solution);

void displayResultAnimated(Solution solution);