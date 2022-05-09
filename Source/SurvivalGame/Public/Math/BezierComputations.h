#pragma once
#include "Point.h"
#include "vector"
#include"Structures/FBiomData.h"

using namespace std;

class BezierComputations
{
public:
	BezierComputations(TMap<EBiomType, FBiomData> BiomDataSet);
	float FilterMap(float HeightMap, float SmoothMap, TEnumAsByte<EBiomType> BiomData);
	void CheckValue(float Value, TEnumAsByte<EBiomType> BiomData);
	TMap<EBiomType, FBiomData> DataSet;
private:
	float BezierLut(float X);
	Point Bezier(float T) const;
	int NearestNeighbourIndex(std::vector<float>& x, float& value);
	float Interpolation1(std::vector<float>& x, std::vector<float>& y, float& x_new);
	static std::vector<float> Linspace(float start_in, float end_in, int num_in);
	float X1;
	float Y1;
	float X2;
	float Y2;
	float A;

};
