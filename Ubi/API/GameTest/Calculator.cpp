#include "stdafx.h"
#include "Calculator.h"
#include <cmath>

using namespace std;
float GetDistance(float x1, float y1, float x2, float y2) {
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}