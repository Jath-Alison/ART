/**
 * @file Vec2.cpp
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Source containing the 2d Vector class
 * @version 1.0-beta
 * @date 07-13-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 * This is the Source file defining the Vec2 class. The class aims to provide
 * utilities for vectors in 2 dimentions, such as locations, travel, and
 * displacement as well as several methods to interact with these vectors.
 */

#include "ART/Vec2.h"

namespace art
{
    Vec2::Vec2() : x(0.0), y(0.0) {}

	double Vec2::magnitude() const
	{
		return std::sqrt(pow(x, 2) + pow(y, 2));
	}

	double Vec2::direction() const
	{
		double angle = 0;
		angle = atan2(x, y);
		return angle;
	}

	Vec2 Vec2::normalize() const
	{
		double scale = 1.f / magnitude();
		Vec2 out = *this * scale;
		return out;
	}

	double Vec2::distTo(Vec2 target) const
	{
		Vec2 dist = target - *this;
		return dist.magnitude();
	}

	double Vec2::angleTo(Vec2 target) const
	{
		Vec2 dist = target - *this;
		return dist.direction();
	}

	Vec2 Vec2::operator+(Vec2 const& obj) const
	{
		Vec2 output = XandY(this->x + obj.x, this->y + obj.y);
		return output;
	}

	Vec2 Vec2::operator-(Vec2 const& obj) const
	{
		Vec2 output = XandY(this->x - obj.x, this->y - obj.y);
		return output;
	}

	Vec2 Vec2::operator*(double const& scale) const
	{
		Vec2 output = XandY(this->x * scale, this->y * scale);
		return output;
	}

	double Vec2::operator*(Vec2 const& other) const
	{
		return  x*other.x + y*other.y ;
	}

	Vec2& Vec2::operator+=(Vec2 const& obj)
	{
		this->x += obj.x;
		this->y += obj.y;
		return *this;
	}

	Vec2& Vec2::operator-=(Vec2 const& obj)
	{
		this->x -= obj.x;
		this->y -= obj.y;
		return *this;
	}

	Vec2& Vec2::operator*=(double const& scale)
	{
		this->x *= scale;
		this->y *= scale;
		return *this;
	}

	Vec2 Vec2::XandY(double x, double y)
	{
		Vec2 output;
		output.x = x;
		output.y = y;
		return output;
	}

	Vec2 Vec2::dirAndMag(double dir, double mag)
	{
		Vec2 output;
		output.x = mag * sin(dir);
		output.y = mag * cos(dir);
		return output;
	}
} // namespace art
