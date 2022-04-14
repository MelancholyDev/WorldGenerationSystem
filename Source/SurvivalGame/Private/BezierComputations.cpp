#include "BezierComputations.h"

#include "Point.h"
#include "math.h"
#include "vector"

Point BezierComputations::Bezier(float X1, float Y1, float X2, float Y2, float A, float T)
{
	Point P1 = Point(0, 0);
	Point P2 = Point(X1, Y1);
	Point P3 = Point(X2, Y2);
	Point P4 = Point(1, A);
	Point Result = pow(1 - T, 3)*P1 +  3 * pow((1 - T), 2) * T * P2 + 3 * (1 - T) * pow(T,2) * P3 +pow(T,3) * P4;
	return Point(0,0);
}

float BezierComputations::FilterMap(float HeightMap, float SmoothMap, float X1, float Y1, float X2, float Y2, float A,
	float B)
{
	// float X = B*HeightMap+(1-B)*SmoothMap;
	// float Y = Bezier(X1,Y1,X2,Y2,A);
	return  1;
}

#include <iostream>
#include <vector>
#include <limits>
#include <cmath>


template<typename Real>
int nearestNeighbourIndex(std::vector<Real> &x, Real &value)
{
	Real dist = std::numeric_limits<Real>::max();
	Real newDist = dist;
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

template<typename Real>
std::vector<Real> interp1(std::vector<Real> &x, std::vector<Real> &y, std::vector<Real> &x_new)
{
	std::vector<Real> y_new;
	Real dx, dy, m, b;
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

	return y_new;
}

int main() {

	// auto res = interp1(x, y, newx);
	// for (auto i: res)
	// 	cout << i << " ";
	// cout << endl;
}
