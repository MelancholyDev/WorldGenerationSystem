#include "Math/BezierComputations.h"

#include "Point.h"
#include "vector"

float BezierComputations::BezierLut(float X1, float Y1, float X2, float Y2, float A, float X)
{
	auto t = Linspace(0, 1, 256);
	std::vector<float> vecX{};
	std::vector<float> vecY{};
	for (int i = 0; i < t.size(); i++)
	{
		Point a = Bezier(X1, Y1, X2, Y2, A, t.at(i));
		vecX.push_back(a.X);
		vecY.push_back(a.Y);
	}
	const float result = Interpolation1(vecX, vecY, X);
	return result;
}

Point BezierComputations::Bezier(float X1, float Y1, float X2, float Y2, float A, float T)
{
	Point P1 = Point(0, 0);
	Point P2 = Point(X1, Y1);
	Point P3 = Point(X2, Y2);
	Point P4 = Point(1, A);
	Point Result = pow(1 - T, 3) * P1 + 3 * pow((1 - T), 2) * T * P2 + 3 * (1 - T) * pow(T, 2) * P3 + pow(T, 3) * P4;
	return Result;
}

float BezierComputations::FilterMap(float HeightMap, float SmoothMap, FBiomData Biom)
{
	float B = Biom.B;
	float X = B * HeightMap + (1 - B) * SmoothMap;
	float Y = BezierLut(Biom.X1, Biom.Y1, Biom.X2, Biom.Y2, Biom.A, X);
	return Y;
}

int NearestNeighbourIndex(std::vector<float>& x, float& value)
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

float Interpolation1(std::vector<float>& x, std::vector<float>& y, float& x_new)
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

std::vector<float> Linspace(float start_in, float end_in, int num_in)
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
