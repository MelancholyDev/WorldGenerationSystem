#pragma once

class Point
{
public:
	float X;
	float Y;
	Point(float X,float Y);
};

inline Point operator + (Point X,Point Y)
{
	return  Point(X.X+Y.X,X.Y+Y.Y);
}

inline Point operator - (Point X,Point Y)
{
	return  Point(X.X-Y.X,X.Y-Y.Y);
}

inline Point operator * (Point X,float Y)
{
	return  Point(X.X*Y,X.Y*Y);
}

inline Point operator * (float X,Point Y)
{
	return  Point(Y.X*X,Y.Y*X);
}