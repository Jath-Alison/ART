/**
 * @file Units.cpp
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Source containing several Unit classes
 * @version 0.1
 * @date 07-13-2024
 *
 * @copyright Copyright (c) 2024
 *
 * This is the source defining various Unit classes. The classes aim to aid in
 * dealing with angles and distances by providing a standard form for other
 * classes to use. That way, any of the other units can be used and converted
 * rather than writing seperate functions for different units.
 */

#include "ART/Units.h"

namespace art
{
    double Length::pixels()
    {
        return m_value;
    }

    double Length::inches()
    {
        return m_value * 0.2f;
    }

    double Length::feet()
    {
        return inches() / 12.f;
    }

    double Length::meters()
    {
        return inches() * .0254f;
    }

    double Length::centimeters()
    {
        return inches() * 2.54f;
    }

    double Length::millimeters()
    {
        return inches() * 25.4f;
    }

    double Length::tiles()
    {
        return inches() / 24.f;
    }

    Length Length::operator=(double const &f)
    {
        Length output;
        output.m_value = f;
        return output;
    }

    Length::operator double()
    {
        return m_value;
    }

    Length::Length() : m_value(0.0) {}

    Length::Length(double f) : m_value(f) {}

    Length Pixels(double pixels)
    {
        Length output;
        output.m_value = pixels;
        return output;
    }

    Length Inches(double inches)
    {
        Length output;
        output.m_value = inches * 5;
        return output;
    }

    Length Feet(double feet)
    {
        Length output;
        output.m_value = feet * 5 * 12;
        return output;
    }

    Length Meters(double meters)
    {
        Length output = Inches(meters);
        output.m_value = output.m_value * 39.3700787402f;
        return output;
    }

    Length Centimeters(double centimeters)
    {
        Length output = Inches(centimeters);
        output.m_value = output.m_value * 0.393700787402f;
        return output;
    }

    Length Millimeters(double millimeters)
    {
        Length output = Inches(millimeters);
        output.m_value = output.m_value * .0393700787402f;
        return output;
    }

    Length Tiles(double tiles)
    {
        Length output = Inches(tiles);
        output.m_value = output.m_value * 24;
        return output;
    }

    Angle Degrees(double degrees)
    {
        Angle output;
        output.m_value = degrees * 3.14159265f / 180;
        return output;
    }

    Angle Radians(double radians)
    {
        Angle output;
        output.m_value = radians;
        return output;
    }

    Angle Revolutions(double revolutions)
    {
        Angle output;
        output.m_value = revolutions * 2.f * 3.14159265f;
        return output;
    }

    Angle::Angle() : m_value(0.0) {}

    Angle::Angle(double f) : m_value(f) {}

    void Angle::constrain()
    {
        while (m_value > 3.14159265f)
        {
            m_value -= 2.f * 3.14159265f;
        }
        while (m_value < -3.14159265f)
        {
            m_value += 2.f * 3.14159265f;
        }
    }

    Angle Angle::operator=(double const &f)
    {
        Angle output;
        output.m_value = f;
        return output;
    }

    Angle::operator double()
    {
        // constrain();
        return m_value;
    }

    double Angle::degrees()
    {
        // constrain();
        return m_value * 180 / 3.14159265f;
    }

    double Angle::radians()
    {
        // constrain();
        return m_value;
    }

    double Angle::revolutions()
    {
        // constrain();
        return m_value / (3.14159265f * 2.f);
    }

    Angle shortestTurnPath(const Angle target)
    {
        Angle angle = target;
        angle.constrain();
        if (std::abs(angle.revolutions()) < .5)
        {
            return angle;
        }
        else
        {
            if (angle.revolutions() > 0)
            {
                return Revolutions(1 - angle.revolutions());
            }
            else
            {
                return Revolutions(1 + angle.revolutions());
            }
        }
        return Angle();
    }

} // namespace art