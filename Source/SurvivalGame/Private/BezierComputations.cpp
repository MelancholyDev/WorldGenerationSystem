#include "BezierComputations.h"

#include "Point.h"
#include "math.h"
#include "vector"

float BezierComputations::BezierLut(float X1, float Y1, float X2, float Y2, float A,float X)
{
	auto t = linspace(0,1,256);
	std::vector<float> vecX{};
	std::vector<float> vecY{};
	for(int i=0;i<t.size();i++)
	{
		Point a = Bezier(X1,Y1,X2,Y2,A,t.at(i));
		vecX.push_back(a.X);
		vecY.push_back(a.Y);
	}
	std::vector<float> result;
	std::vector<float> buffer{X};
	result = interp1(vecX,vecY,buffer);
	return result.at(0);
}

Point BezierComputations::Bezier(float X1, float Y1, float X2, float Y2, float A, float T)
{
	Point P1 = Point(0, 0);
	Point P2 = Point(X1, Y1);
	Point P3 = Point(X2, Y2);
	Point P4 = Point(1, A);
	Point Result = pow(1 - T, 3)*P1 +  3 * pow((1 - T), 2) * T * P2 + 3 * (1 - T) * pow(T,2) * P3 +pow(T,3) * P4;
	return Result;
}

float BezierComputations::FilterMap(float HeightMap, float SmoothMap, float X1, float Y1, float X2, float Y2, float A,
	float B)
{
	float X = B*HeightMap+(1-B)*SmoothMap;
	FString num = FString::Printf(TEXT("%f"),X);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, num);
	float Y = BezierLut(X1, Y1, X2, Y2, A, X);
	return Y;
}
int nearestNeighbourIndex(std::vector<float> &x, float &value)
{
	float dist = std::numeric_limits<float>::max();
	float newDist = dist;
	size_t idx = 0;

	for (size_t i = 0; i < x.size(); ++i) {
		newDist = std::abs(value - x[i]);
		if (newDist <= dist) {
			dist = newDist;
			idx = i;
		}
	}

	return idx;
}

std::vector<float> interp1(std::vector<float> &x, std::vector<float> &y, std::vector<float> &x_new)
{
	FString num = FString::Printf(TEXT("%f"),x_new.at(0));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, num);
	std::vector<float> y_new;
	float dx, dy, m, b;
	size_t x_max_idx = x.size() - 1;
	size_t x_new_size = x_new.size();

	y_new.reserve(x_new_size);

	for (size_t i = 0; i < x_new_size; ++i)
	{
		size_t idx = nearestNeighbourIndex(x, x_new[i]);

		if (x[idx] > x_new[i])
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

		y_new.push_back(x_new[i] * m + b);
	}

	// FString num = FString::Printf(TEXT("%f"),y_new.at(0));
 //    			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, num);
	return y_new;
}

std::vector<float> linspace(float start_in, float end_in, int num_in)
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

	for(int i=0; i < num-1; ++i)
	{
		linspaced.push_back(start + delta * i);
	}
	linspaced.push_back(end);
	return linspaced;
}