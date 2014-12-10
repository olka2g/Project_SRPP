#pragma once
#include "DataTypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <vector>
#include <string>
using namespace std;
#include <Windows.h>
#include <iostream>

string chooseFile();
CitiesData loadFile(string filePath);
