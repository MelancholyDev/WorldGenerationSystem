#pragma once
#include "Point.h"
#include "vector"
#include"Structures/FBiomData.h"

using namespace std;
class BezierComputations
{
public:
	static float BezierLut(float X1,float Y1,float X2,float Y2,float A,float X);
	static Point Bezier(float X1, float Y1, float X2, float Y2, float A, float T);
	static float FilterMap(float HeightMap,float SmoothMap,FBiomData Biom);
};


int NearestNeighbourIndex(std::vector<float> &x, float &value);
float  Interpolation1(std::vector<float> &x, std::vector<float> &y, float &x_new);
std::vector<float> Linspace(float start_in, float end_in, int num_in);