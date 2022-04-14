#pragma once
#include "Point.h"
class BezierComputations
{
public:
	static Point BezierLut(float X1,float Y1,float X2,float Y2,float A);
	static Point Bezier(float X1, float Y1, float X2, float Y2, float A, float T);
	static float FilterMap(float HeightMap,float SmoothMap,float X1,float Y1,float X2,float Y3,float A,float B);
};
