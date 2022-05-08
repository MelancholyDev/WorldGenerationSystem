#pragma once

#include <vector>
#include <map>

class DiamondSquareMapGeneration
{
public:
	static void generateWorld(float** tiles,int size);
private:
	static double fRand2(double fMin, double fMax);
	static void timeReset();
};