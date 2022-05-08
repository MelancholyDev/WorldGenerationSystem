#pragma once


class DiamondSquareMapGeneration
{
public:
	static void generateWorld(float** tiles,int size);
	static double fRand2(float fMin, float fMax);
private:
	static void timeReset();
};