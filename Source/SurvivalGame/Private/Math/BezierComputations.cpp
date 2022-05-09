#include "Math/BezierComputations.h"

#include "Point.h"
#include "vector"

float BezierComputations::BezierLut(float X)
{
	auto t = Linspace(0, 1, 256);
	std::vector<float> vecX{};
	std::vector<float> vecY{};
	for (int i = 0; i < t.size(); i++)
	{
		Point a = Bezier(t.at(i));
		vecX.push_back(a.X);
		vecY.push_back(a.Y);
	}
	const float result = Interpolation1(vecX, vecY, X);
	return result;
}

Point BezierComputations::Bezier( float T) const
{
	Point P1 = Point(0, 0);
	Point P2 = Point(X1, Y1);
	Point P3 = Point(X2, Y2);
	Point P4 = Point(1, A);
	Point Result = pow(1 - T, 3) * P1 + 3 * pow((1 - T), 2) * T * P2 + 3 * (1 - T) * pow(T, 2) * P3 + pow(T, 3) * P4;
	return Result;
}

BezierComputations::BezierComputations(TMap<EBiomType, FBiomData> BiomDataSet)
{
	DataSet=BiomDataSet;
	X1 =0;
	X2=0;
	Y1=0;
	Y2=0;
	A=0;
}

float BezierComputations::FilterMap(float HeightMap, float SmoothMap, TEnumAsByte<EBiomType> BiomData)
{
	FBiomData* Biom = DataSet.Find(BiomData);
	float B = Biom->B;
	X1 = Biom->X1;
	X2 = Biom->X2;
	Y1 = Biom->Y1;
	Y2 = Biom->Y2;
	A = Biom->A;
	float X = B * HeightMap + (1 - B) * SmoothMap;
	float Y = BezierLut(X);
	return Y;
}

void BezierComputations::CheckValue(float Value,TEnumAsByte<EBiomType> BiomData)
{
	FBiomData* Biom = DataSet.Find(BiomData);
	Biom->CheckValue(Value);
}

int BezierComputations::NearestNeighbourIndex(std::vector<float>& x, float& value)
{
	float dist = std::numeric_limits<float>::max();
	float newDist;
	size_t idx = 0;

	for (size_t i = 0; i < x.size(); ++i)
	{
		newDist = std::abs(value - x[i]);
		if (newDist <= dist)
		{
			dist = newDist;
			idx = i;
		}
	}

	return idx;
}

float BezierComputations::Interpolation1(std::vector<float>& x, std::vector<float>& y, float& x_new)
{
	float y_new;
	float dx, dy, m, b;
	size_t x_max_idx = x.size() - 1;
	size_t idx = NearestNeighbourIndex(x, x_new);
	if (x[idx] > x_new)
	{
		dx = idx > 0 ? (x[idx] - x[idx - 1]) : (x[idx + 1] - x[idx]);
		dy = idx > 0 ? (y[idx] - y[idx - 1]) : (y[idx + 1] - y[idx]);
	}
	else
	{
		dx = idx < x_max_idx ? (x[idx + 1] - x[idx]) : (x[idx] - x[idx - 1]);
		dy = idx < x_max_idx ? (y[idx + 1] - y[idx]) : (y[idx] - y[idx - 1]);
	}
	m = dy / dx;
	b = y[idx] - x[idx] * m;
	y_new = x_new * m + b;

	return y_new;
}

std::vector<float> BezierComputations::Linspace(float start_in, float end_in, int num_in)
{
	std::vector<float> linspaced;

	double start = start_in;
	double end = end_in;
	double num = num_in;

	if (num == 0) { return linspaced; }
	if (num == 1)
	{
		linspaced.push_back(start);
		return linspaced;
	}

	double delta = (end - start) / (num - 1);

	for (int i = 0; i < num - 1; ++i)
	{
		linspaced.push_back(start + delta * i);
	}
	linspaced.push_back(end);
	return linspaced;
}

