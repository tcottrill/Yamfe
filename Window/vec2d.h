#ifndef __V2D__
#define __V2D__

#pragma once

#include <cmath>

class Vector2D
{
public:
	Vector2D() : x(0), y(0) {}
	Vector2D(float x, float y) : x(x), y(y) {}

	Vector2D operator-(const Vector2D &other) const
	{
		return Vector2D(x - other.x, y - other.y);
	}

	bool WithinDistanceOf(const Vector2D &other, int distance) const
	{
		//Pygorean's theorum: A^2 + B^2 = C^2
		int horizontal = (int) (this->x - other.x); //(this->x - other.x) could be negative, but multiplying it by itself will always be positive anyway.
		horizontal *= horizontal;

		int vertical = (int) (this->y - other.y);
		vertical *= vertical;

		//Square the distance.
		distance *= distance;

		//Use the squared distance for comparison, instead of getting the square root.
		return (horizontal + vertical) <= distance;
	}

	int DistanceFrom(const Vector2D &other)
	{
		//Pygorean's theorum: A^2 + B^2 = C^2
		int horizontal = (int) (this->x - other.x); //(this->x - other.x) could be negative, but multiplying it by itself will always be positive anyway.
		horizontal *= horizontal;

		int vertical = (int) (this->y - other.y);
		vertical *= vertical;

		return std::sqrtf((float) (horizontal + vertical));
	}


	float dot(const Vector2D &other) const
	{
		return x * other.x + y * other.y;
	}

	Vector2D perp() const
	{
		return Vector2D(-y, x);
	}

	float x, y;
};


#endif